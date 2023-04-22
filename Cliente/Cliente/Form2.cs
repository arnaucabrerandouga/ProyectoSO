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

namespace WindowsFormsApplication1
{
    public partial class Form2 : Form
    {
        Socket server;
        int port = 9070;
        string Jugador;
        public Form2(string Jugador)
        {
            this.Jugador=Jugador;
            InitializeComponent();

            this.FormClosing += new FormClosingEventHandler(Form2_FormClosing);
            

        }

        private void Form2_Load(object sender, EventArgs e)
        {
            // Creamos un IPEndPoint con el ip del servidor y puerto del sevidor al que conectamos
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, port);

            //Creamos el socket 
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            server.Connect(ipep);//Intentamos conectar el socket
            MessageBox.Show("Conectado");

            string mensaje = "5/" + this.Jugador;
            // Enviamos al servidor la consulta
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Recibimos la respuesta del servidor
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            label1.Text = mensaje;


        }

        private void buttonConsulta1_Click(object sender, EventArgs e)
        {
            try
            {

                string mensaje = "3/";
                // Enviamos al servidor la consulta
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];

                MessageBox.Show(mensaje);
               

            }
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }
        }

        private void Form2_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("¿Está seguro de que desea salir?", "Confirmar salida", MessageBoxButtons.YesNo) == DialogResult.No)
            {
                e.Cancel = true; // cancela el cierre de la ventana

                string mensaje = "6/" + this.Jugador;
                // Enviamos al servidor la consulta
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                // Nos desconectamos
                server.Shutdown(SocketShutdown.Both);
                server.Close();
            }
        }
        
    }
}
