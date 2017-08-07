using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace WMO_AutoLogin
{
    public partial class WMOAutoLogin : Form
    {
        public WMOAutoLogin()
        {
            InitializeComponent();
        }

        private void WMOBrowser_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            Console.WriteLine("Page Loaded: " + WMOBrowser.Url.AbsoluteUri); // For debugging, print current URL
            if (WMOBrowser.Url.AbsoluteUri == "https://authn.walmartone.com/login.aspx") // starting point
            {
                HtmlDocument document = WMOBrowser.Document;
                HtmlElement username = document.GetElementById("uxUserName");
                HtmlElement password = document.GetElementById("uxPassword");
                HtmlElement login = document.GetElementById("SubmitCreds");

                if (!File.Exists("Credentials.txt")) // if credentials exists
                {
                    Console.WriteLine("File containing credentials does not exist in executable directory.");
                    Environment.Exit(-2); // File cannot be opened error
                }
                StreamReader fStream = new StreamReader("Credentials.txt"); // read file for credentials
                if (fStream == null)
                {
                    Console.WriteLine("Unable to open file containing credentials.");
                    Environment.Exit(-2); // File cannot be opened error
                }
                string line; // current line
                string User_Username = null, User_Password = null;
                while ((line = fStream.ReadLine()) != null)
                { 
                    if (line.StartsWith("uxUserName:"))
                    {
                        User_Username = line.Substring(11); // risky, but format should remain the same so we should be good
                    } // TODO: fix lazy noob code above
                    else if (line.StartsWith("uxPassword:"))
                    {
                        User_Password = line.Substring(11); // stop
                    }
                }
                fStream.Close();

                if (User_Username == null || User_Password == null)
                {
                    Console.WriteLine("Failed to read file containing credentials.");
                    Environment.Exit(-2); // File could not be opened error
                }

                username.SetAttribute("value", User_Username);
                password.SetAttribute("value", User_Password);
                login.InvokeMember("click"); // login
            }
            else if (WMOBrowser.Url.AbsoluteUri == "https://authn.walmartone.com/login.aspx?error=1")
            {
                Console.WriteLine("Failed to login, credentials are invalid. Username not found.");
                Environment.Exit(-3); // Invalid credentials error
            }
            else if (WMOBrowser.Url.AbsoluteUri == "https://authn.walmartone.com/login.aspx?error=2")
            {
                Console.WriteLine("Failed to login, credentials are invalid. Password is incorrect.");
                Environment.Exit(-3); // Invalid credentials error
            }
            else if (WMOBrowser.Url.AbsoluteUri == "https://us3.walmartone.com/")
            {               
                WMOBrowser.Url = new Uri(WMOBrowser.Url, "https://sso.walmartone.com/wmb/redirect.html?TARGET=https:%2F%2Flogin.walmartone.com%2Fonlineschedule/schedule/FullSchedule.aspx");
            }
            else if (WMOBrowser.Url.AbsoluteUri == "https://pfedprod.walmartone.com/idp/startSSO.ping?PartnerSpId=https://www.walmartone.com&")
            { } // must be here to avoid hitting else and timing out for no reason
            else if (WMOBrowser.Url.AbsoluteUri == "https://www.walmartone.com/saml/ssoservice.aspx")
            { } // same as above
            else if (WMOBrowser.Url.AbsoluteUri == "https://pfedprod.walmartone.com/idp/startSSO.ping?PartnerSpId=SP_Walmart_Benefits&TARGET=https://login.walmartone.com/onlineschedule/schedule/FullSchedule.aspx")
            {
                WMOBrowser.Url = new Uri(WMOBrowser.Url, "https://login.walmartone.com/onlineschedule/schedule/FullSchedule.aspx");
            }
            else if (WMOBrowser.Url.AbsoluteUri == "https://login.walmartone.com/onlineschedule/schedule/FullSchedule.aspx")
            {
                HtmlDocument document = WMOBrowser.Document;
                document.InvokeScript("showMonthView");
                HtmlElement Schedule = document.GetElementById("calTab");
                if (Schedule == null) // try to fetch schedule
                {
                    Console.WriteLine("Failed to load schedule");
                    Environment.Exit(-4); // Failed to load schedule error code (probably means unable to reach wmo)
                }
                else
                { // "download" html doc, but only the schedule part because I am lazy and want less work later on
                    Console.WriteLine("Schedule Loaded");
                    File.Delete("Schedule.html");
                    StreamWriter ScheduleWriter = new StreamWriter("Schedule.html");
                    ScheduleWriter.Write(Schedule.InnerHtml);
                    ScheduleWriter.Close();
                    Console.WriteLine("Current Month Downloaded");

                    object[] parameters = new object[1];
                    parameters[0] = 1;
                    document.InvokeScript("updateMonth", parameters);

                }
            }
            else if (WMOBrowser.Url.AbsoluteUri.Contains("https://login.walmartone.com/onlineschedule/schedule/FullSchedule.aspx?"))
            {
                HtmlDocument document = WMOBrowser.Document;
                document.InvokeScript("showMonthView");
                HtmlElement Schedule = document.GetElementById("calTab");
                if (Schedule == null) // get next months schedule so that if it is end of month, you still know your 2-3 week schedule
                {
                    Console.WriteLine("Failed to load schedule");
                    Environment.Exit(-4); // Failed to load schedule error code (probably means unable to reach wmo)
                }
                else
                {
                    Console.WriteLine("Schedule Loaded");
                    File.Delete("NSchedule.html");
                    StreamWriter ScheduleWriter = new StreamWriter("NSchedule.html");
                    ScheduleWriter.Write(Schedule.InnerHtml);
                    ScheduleWriter.Close();
                    Console.WriteLine("Next Month Downloaded");
                    Environment.Exit(0); // Normal exit code: Everything is okay
                }
            }
            else
            { // oh no wmo is sh*t... who knew
                Console.WriteLine("Connection timed out");
                Environment.Exit(-5); // Connection timed out error code
            }
        }
    }
}
