using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;
using System.Timers;
using System.Xml;

namespace WIFI1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            dt.Columns.Add("ID", typeof(string));
            dt.Columns.Add("FirstWifi", typeof(string));
            dt.Columns.Add("SecondWifi", typeof(string));
            dt.Columns.Add("ThirdWifi", typeof(string));
            dt.Columns.Add("ForthWifi", typeof(string));//datatable  列
            dataGridView1.DataSource = dt.DefaultView;  //绑定显示datatable
            dataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.DisplayedCells;//自动调整列
            //IPAddress ipAddress = IPAddress.Any;
        }
        DataTable dt = new DataTable("wifi");           //内存datatable
        Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);//初始化socket

        private void button1_Click(object sender, EventArgs e)//TCP连接
        {
            if((textBox1.Text!="")||(textBox2.Text!=""))
            {
                IPAddress ip = IPAddress.Parse(textBox1.Text);
                try
                {
                    s.Connect(ip, Convert.ToInt16(textBox2.Text));
                    MessageBox.Show("连接中");
                }
                catch
                {
                    MessageBox.Show("连接失败");
                }
                try
                {
                    if (s.Connected == true)
                    {
                        MessageBox.Show("连接成功");
                        Thread thread = new Thread(new ThreadStart(TimerMange));//连接成功开线程读
                        thread.Start();
                    }
                    else
                    {
                        MessageBox.Show("连接失败");
                    }
                }
                catch
                {
                    MessageBox.Show("状态出错");
                }
            }
            else
            {
                MessageBox.Show("请输入端口号");
            }
            
        }

        public delegate void myinvoke();//委托显示
        byte[] res = new byte[1024];//读数组
        void TimerMange()//线程运行程序
        {
            while (true)
            {
                TM();
            }
        }
        void TM()//等待读取
        {
            int count = 0;
            if (s.Connected == true)
            {
                do
                {
                    count = s.Receive(res, res.Length, 0);
                } while (count == 0);
                myinvoke mi = new myinvoke(socket_rev);//调用委托
                textBox4.Invoke(mi);
            }
            else
            {
                
                //mi
            }
        }
        private void socket_rev()//委托函数  数据处理
        {
            string s,a,b,c,d,e;
            int i;
            s = Encoding.ASCII.GetString(res, 0,res.Length );//读取数据转换字符串
            textBox4.Text =s;
            DataRow  dr = dt.NewRow ();                     //新建行
            a = s.Substring(0, s.IndexOf(","));
            dr[0] = a;
            s = s.Remove(0, s.IndexOf(",")+1);
            b = s.Substring(0, s.IndexOf(")"));
            b = b.Remove(0, 8);
            dr[1] = b;
            s = s.Remove(0, s.IndexOf(")") + 3);
            c = s.Substring (0, s.IndexOf (")"));
            c = c.Remove (0, 8);
            dr[2] = c;
            s = s.Remove(0, s.IndexOf(")") + 3);
            d = s.Substring(0, s.IndexOf(")"));
            d = d.Remove(0, 8);
            dr[3] =d;
            s = s.Remove(0, s.IndexOf(")") + 3);
            e = s.Substring(0, s.IndexOf(")"));
            e = e.Remove(0, 8);
            dr[4] = e;
            dt.Rows.Add(dr);                                //加入行
        }
        
        private void button3_Click(object sender, EventArgs e)
        {
            dt.WriteXml(Application.StartupPath + "wifi.xml");//存储
        }

        private void button4_Click(object sender, EventArgs e)
        {
            dt.ReadXml(Application.StartupPath + "wifi.xml");//载入
        }
    }
    

}
