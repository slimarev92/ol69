using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ConsumeRestFullService
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void btnBind_Click(object sender, EventArgs e)
        {
            WebClient proxy = new WebClient();
            proxy.DownloadStringAsync(new Uri("http://localhost:1678/Service1.svc/Students"));
            proxy.DownloadStringCompleted +=proxy_DownloadStringCompleted;
        }

        void proxy_DownloadStringCompleted(object sender, DownloadStringCompletedEventArgs e)
        {
            Stream stream = new MemoryStream(Encoding.Unicode.GetBytes(e.Result));
            DataContractJsonSerializer obj = new DataContractJsonSerializer(typeof(List<Student> ));
            List<Student> result = obj.ReadObject(stream) as List<Student>;

            foreach (var x in result)
            {
                label1.Text = result[0].ID.ToString();
                label2.Text = result[0].Name.ToString();
            }

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }
    }
}
