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

namespace Sender
{
    public partial class Sender : Form
    {
        Socket server = null;
        Socket client = null;
        Thread thServerStart = null;
        Thread thImageSend = null;
        
        public Sender()
        {
            InitializeComponent();
        }

        private void Sender_FormClosing(object sender, FormClosingEventArgs e)
        {
            ServerStop();
        }

        /// <summary>
        /// Server Start & Stop
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnOperate_Click(object sender, EventArgs e)
        {
            if (btnOperate.Text == "Start")
            {
                thServerStart = new Thread(new ThreadStart(ServerStart));
                thServerStart.Start();
            }
            else if (btnOperate.Text == "Stop")
            {
                ServerStop();
            }
        }

        /// <summary>
        /// ServerStart
        /// </summary>
        public void ServerStart()
        {
            try
            {
                btnOperate.Text = "Stop";
                btnOperate.BackColor = Color.Red;

                IPEndPoint ipep = new IPEndPoint(IPAddress.Any, Int16.Parse(txtPort.Text));
                server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                server.Bind(ipep);
                server.Listen(10);
                this.client = server.Accept();

                if (client != null && client.Connected)
                {
                    thImageSend = new Thread(new ThreadStart(ImageSend));
                    thImageSend.Start();
                }
            }
            catch (Exception ex)
            {
                Trace.WriteLine("ServerStart() : " + ex.Message + "\r\n");
            }
        }

        /// <summary>
        /// ServerStop
        /// </summary>
        public void ServerStop()
        {
            try
            {
                btnOperate.Text = "Start";
                btnOperate.BackColor = SystemColors.Control;

                // Server & Client socket close.
                if (client != null && client.Connected)
                {
                    client.Close();
                }
                server.Close();

                // ImageSend thread abort.
                if ((thImageSend != null) && (thImageSend.IsAlive))
                {
                    thImageSend.Abort();
                }

                // ServerStart Thread abort.
                if ((thServerStart != null) && (thServerStart.IsAlive))
                {
                    thServerStart.Abort();
                }
            }
            catch (Exception ex)
            {
                Trace.WriteLine("ServerStop() : " + ex.Message + "\r\n");
            }
        }

        /// <summary>
        /// Image capture and Send
        /// </summary>
        public void ImageSend()
        {
            try
            {
                int width = Screen.AllScreens[0].Bounds.Width;
                int height = Screen.AllScreens[0].Bounds.Height;

                Size size = new Size(width, height);
                Bitmap bitmap = new Bitmap(width, height);
                Graphics g = Graphics.FromImage(bitmap);

                while (true)
                {
                    g.CopyFromScreen(0, 0, 0, 0, size);

                    MemoryStream ms = new MemoryStream();
                    bitmap.Save(ms, ImageFormat.Png);
                    ms.Position = 0;

                    byte[] data = ms.ToArray();
                    SendData(data);

                    Thread.Sleep(300);
                }
            }
            catch (Exception ex)
            {
                Trace.WriteLine("ImageSend() : " + ex.Message + "\r\n");
                ServerStop();
                return;
            }
        }

        /// <summary>
        /// 상대방에게 바이트 배열 전송하기
        /// </summary>
        /// <param name="data">전송할 바이트 배열</param>
        private void SendData(byte[] data)
        {
            try
            {
                int total = 0;           // 전송된 총 크기
                int size = data.Length;  // 전송할 바이트 배열의 크기
                int left_data = size;    // 남은 데이터 량
                int send_data = 0;       // 전송된 데이터 크기

                // 전송할 실제 데이터의 크기 전달
                byte[] data_size = new byte[4];   // 정수형태로 데이터 크기 전송
                data_size = BitConverter.GetBytes(size);
                send_data = this.client.Send(data_size);

                // 실제 데이터 전송
                while (total < size)
                {  // 상대방에게 데이터 전송하기 
                    send_data = this.client.Send(data, total, left_data, SocketFlags.None);
                    total += send_data;
                    left_data -= send_data;
                }
            }
            catch (Exception ex)
            {
                // 데이터 전송중 예외가 발생하면 에러 메시지 출력
                Trace.WriteLine("SendData() : " + ex.Message + "\r\n");
                ServerStop();
                return;
            }
        }
    }
}
