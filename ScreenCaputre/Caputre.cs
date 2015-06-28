using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;

namespace ScreenCaputre
{
    public partial class Caputre : Form
    {
        Bitmap bitmap = null;
        public Caputre()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 윈도우 전체화면 캡쳐.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void captureMenu_Click(object sender, EventArgs e)
        {
            int width = Screen.PrimaryScreen.Bounds.Width;
            int height = Screen.PrimaryScreen.Bounds.Height;

            Size size = new Size(width, height);
            bitmap = new Bitmap(width, height);
            Graphics g = Graphics.FromImage(bitmap);
            g.CopyFromScreen(0, 0, 0, 0, size);

            pictureBox.Image = bitmap;
        }

        /// <summary>
        /// 캡쳐된 화면 파일로 저장.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void saveMenu_Click(object sender, EventArgs e)
        {
            if (bitmap != null)
            {
                saveFileDialog.Filter = "JPEG|*.jpg|BMP|*.bmp|PNG|*.png";
                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    switch (saveFileDialog.FilterIndex)
                    {
                        case 1:
                            bitmap.Save(saveFileDialog.FileName, ImageFormat.Jpeg);
                            break;
                        case 2:
                            bitmap.Save(saveFileDialog.FileName, ImageFormat.Bmp);
                            break;
                        case 3:
                            bitmap.Save(saveFileDialog.FileName, ImageFormat.Png);
                            break;
                    }
                }
            }
            else
            {
                MessageBox.Show("Caputre first.", "Error");
            }
        }

        private void exitMenu_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
