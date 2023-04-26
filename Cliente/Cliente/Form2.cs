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

namespace WindowsFormsApplication1
{
    public partial class Form2 : Form
    {
        Socket server;
        Thread atender;
        int port = 9060;
        string direccion = "192.168.56.102";
        string Jugador;
        public Form2(string Jugador)
        {
            this.Jugador = Jugador;
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false; //Necesario para que los elementos de los formularios puedan ser
            //accedidos desde threads a los que los crearon
            this.FormClosing += new FormClosingEventHandler(Form2_FormClosing);


        }

        private void AtenderServidor()
        {
            //try
            //{

            while (true)
            {
                //Recibimos un mensaje del servidor y creamos un vector con su contenido
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                string men = Encoding.ASCII.GetString(msg2);
                MessageBox.Show(men);
                string[] res = men.Split('/');
                //Usamos el res[0] para averiguar quien es el destinatario del mensaje
                int codigo = Convert.ToInt32(res[0]);
                string r;
                r = res[1].Split('\0')[0];
                MessageBox.Show(r);
                switch (codigo)
                {
                    case 0: //Desconexion
                        server.Shutdown(SocketShutdown.Both);
                        server.Close();
                        atender.Abort();
                        break;
                    case 1: //Nos conectamos con un usuario existente
                        MessageBox.Show(r);
                        break;
                    case 2: //Creamos un nuevo usuario
                        MessageBox.Show(r);
                        break;
                    case 6: //Lista conectados
                        //Amigos_DataGrid(res);
                        break;

                        /*case 8: //Contador de peticiones atendidas
                            contLbl.Text = r;
                            break;*/
                }
            }
        }

        public void ConectarServidor(string direccion, int puerto)
        {
            // Creamos un IPEndPoint con el ip del servidor y puerto del sevidor al que conectamos
            IPAddress direc = IPAddress.Parse(direccion);
            IPEndPoint ipep = new IPEndPoint(direc, puerto);

            //Creamos el socket 
            Socket server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            server.Connect(ipep); // Intentamos conectar el socket

            // Aquí puedes realizar cualquier acción necesaria con la conexión establecida, como enviar o recibir datos
        }
        private void Form2_Load(object sender, EventArgs e)
        {
            //// Creamos un IPEndPoint con el ip del servidor y puerto del sevidor al que conectamos
            //IPAddress direc = IPAddress.Parse(direccion);
            //IPEndPoint ipep = new IPEndPoint(direc, port);

            ////Creamos el socket 
            //server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            //server.Connect(ipep);//Intentamos conectar el socket
            //MessageBox.Show("Conectado");

            //pongo en marcha el thread que atenderá los mensajes del servidor
            ThreadStart ts = delegate { AtenderServidor(); };
            atender = new Thread(ts);
            atender.Start();

            //string mensaje = "6/";
            //// Enviamos al servidor la consulta
            //byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            //server.Send(msg);

            //Recibimos la respuesta del servidor
            //byte[] msg2 = new byte[80];
            //server.Receive(msg2);
            //mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            //label1.Text = mensaje;


        }

        private void buttonConsulta1_Click(object sender, EventArgs e)
        {
            //try
            //{

            //    string mensaje = "3/";
            //    // Enviamos al servidor la consulta
            //    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            //    server.Send(msg);
            //}
            //catch (SocketException ex)
            //{
            //    //Si hay excepcion imprimimos error y salimos del programa con return 
            //    MessageBox.Show("No he podido conectar con el servidor");
            //    return;
            //}
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
                atender.Abort();
                server.Shutdown(SocketShutdown.Both);
                server.Close();
            }
        }

        private void buttonConsulta2_Click(object sender, EventArgs e)
        {

        }
    }
}
