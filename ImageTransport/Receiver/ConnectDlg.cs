using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Receiver
{
    public partial class ConnectDlg : Form
    {
        public ConnectDlg()
        {
            InitializeComponent();
        }

        public String IP
        {
            get { return txtIP.Text; }
        }

        public String Port
        {
            get { return txtPort.Text; }
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {

        }
    }
}
