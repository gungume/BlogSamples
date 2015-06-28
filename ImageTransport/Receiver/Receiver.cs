using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;
using System.Drawing.Imaging;
using System.Diagnostics;

namespace Receiver
{
    public partial class Receiver : Form
    {
        Socket client = null;
        Thread thImageReceive = null;

        public Receiver()
        {
            InitializeComponent();
        }

        private void Receiver_Load(object sender, EventArgs e)
        {
            CheckForIllegalCrossThreadCalls = false;
        }

        private void Receiver_FormClosing(object sender, FormClosingEventArgs e)
        {
            Disconnect();
        }

        private void connectMenu_Click(object sender, EventArgs e)
        {
            ConnectDlg dlg = new ConnectDlg();
            dlg.Location = new Point(this.Left, this.Top);

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                if (dlg.IP == "")
                {
                    MessageBox.Show("Input IP Number", "Warning");
                    return;
                }
                if (dlg.Port == "")
                {
                    MessageBox.Show("Input Port Number", "Warning");
                    return;
                }

                if (Connect(dlg.IP, int.Parse(dlg.Port)) == true)
                {
                    if (client != null && client.Connected)
                    {
                        thImageReceive = new Thread(new ThreadStart(ImageReceive));
                        thImageReceive.Start();

                        connectMenu.Enabled = false;
                        disConnectMenu.Enabled = true;
                    }
                }
            }
        }

        private void disConnectMenu_Click(object sender, EventArgs e)
        {
            Disconnect();
        }

        private void exitMenu_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        /// <summary>
        /// Connect Server
        /// </summary>
        /// <param name="ip">Server IP address</param>
        /// <param name="port">Server Port number</param>
        /// <returns></returns>
        public bool Connect(string ip, int port)
        {
            try
            {
                IPEndPoint ipep = new IPEndPoint(IPAddress.Parse(ip), port);
                client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                client.Connect(ipep);

                return true;
            }
            catch (Exception ex)
            {
                Trace.WriteLine("Connect() : " + ex.Message + "\r\n");
                return false;
            }
        }

        /// <summary>
        /// Disconnect Server
        /// </summary>
        public void Disconnect()
        {
            try
            {
                pictureBox.Image = null;
                connectMenu.Enabled = true;
                disConnectMenu.Enabled = false;

                // Client socket close.
                if (client != null && client.Connected)
                {
                    client.Close();
                }

                // ImageReceive thread abort.
                if ((thImageReceive != null) && (thImageReceive.IsAlive))
                {
                    thImageReceive.Abort();
                    thImageReceive = null;
                }
            }
            catch (Exception ex)
            {
                Trace.WriteLine("Disconnect() : " + ex.Message + "\r\n");
            }
        }

        /// <summary>
        /// Captured image receive
        /// </summary>
        public void ImageReceive()
        {
            try
            {
                while (true)
                {
                    if (client != null && client.Connected)
                    {
                        byte[] data = ReceiveData();

                        MemoryStream ms = new MemoryStream();
                        ms.Write(data, 0, (int)data.Length);

                        Bitmap bitmap = new Bitmap(ms);
                        pictureBox.Image = bitmap;

                        Thread.Sleep(100);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            catch (Exception ex)
            {
                Trace.WriteLine("ImageReceive() : " + ex.Message + "\r\n");
                Disconnect();
            }
        }

        /// <summary>
        /// 상대방이 보낸 데이터 수신하기
        /// </summary>		
        /// <returns>수신한 데이터의 바이트 배열</returns>
        private byte[] ReceiveData()
        {
            try
            {
                int total = 0;        // 수신된 데이터 총량
                int size = 0;        // 수신할 데이터 크기
                int left_data = 0;   // 남은 데이터 크기
                int recv_data = 0;  // 수신한 데이터 크기

                // 수신할 데이터 크기 알아내기   
                byte[] data_size = new byte[4];
                recv_data = this.client.Receive(data_size, 0, 4, SocketFlags.None);
                size = BitConverter.ToInt32(data_size, 0);
                left_data = size;

                byte[] data = new byte[size];  // 바이트 배열 생성
                // 서버에서 전송한 실제 데이터 수신
                while (total < size)
                {  // 상대방이 전송한 데이터를 읽어옴
                    recv_data = this.client.Receive(data, total, left_data, 0);
                    if (recv_data == 0) break;
                    total += recv_data;
                    left_data -= recv_data;
                }
                return data;
            }
            catch (Exception ex)
            {
                // 데이터 수신중 예외가 발생하면 에러 메시지 출력
                Trace.WriteLine("ReceiveData() : " + ex.Message + "\r\n");
                Disconnect();
                return null;
            }
        }
    }
}
