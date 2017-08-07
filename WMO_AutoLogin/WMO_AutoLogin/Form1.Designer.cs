namespace WMO_AutoLogin
{
    partial class WMOAutoLogin
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.WMOBrowser = new System.Windows.Forms.WebBrowser();
            this.SuspendLayout();
            // 
            // WMOBrowser
            // 
            this.WMOBrowser.AllowWebBrowserDrop = false;
            this.WMOBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.WMOBrowser.IsWebBrowserContextMenuEnabled = false;
            this.WMOBrowser.Location = new System.Drawing.Point(0, 0);
            this.WMOBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.WMOBrowser.Name = "WMOBrowser";
            this.WMOBrowser.ScriptErrorsSuppressed = true;
            this.WMOBrowser.Size = new System.Drawing.Size(784, 561);
            this.WMOBrowser.TabIndex = 0;
            this.WMOBrowser.Url = new System.Uri("https://authn.walmartone.com/login.aspx", System.UriKind.Absolute);
            this.WMOBrowser.DocumentCompleted += new System.Windows.Forms.WebBrowserDocumentCompletedEventHandler(this.WMOBrowser_DocumentCompleted);
            // 
            // WMOAutoLogin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 561);
            this.ControlBox = false;
            this.Controls.Add(this.WMOBrowser);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "WMOAutoLogin";
            this.Opacity = 0D;
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "WMO AutoLogin";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.WebBrowser WMOBrowser;
    }
}

