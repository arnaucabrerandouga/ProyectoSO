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
    public partial class Form1 : Form
    {
        Socket server;
        int port = 9070;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void buttonIniciarSesion_Click(object sender, EventArgs e)
        {
            try
            {

                if (String.IsNullOrEmpty(textBoxUsuario.Text) || String.IsNullOrEmpty(textBoxContraseña.Text)) // nome she ficat nom o contra  avisame
                {
                    MessageBox.Show("Escribe un nombre y una contraseña");
                }
                else
                {
                    // Creamos un IPEndPoint con el ip del servidor y puerto del sevidor al que conectamos
                    IPAddress direc = IPAddress.Parse("192.168.56.102");
                    IPEndPoint ipep = new IPEndPoint(direc, port);

                    //Creamos el socket 
                    server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                    server.Connect(ipep);//Intentamos conectar el socket
                    MessageBox.Show("Conectado");

                    string mensaje = "1/" + textBoxUsuario.Text + "/" + textBoxContraseña.Text;
                    // Enviamos al servidor el usuario y la contraseña
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];

                    if (mensaje == "BIEN") //ha anat be obram un form per fer peticions
                    {
                        string Jugador = textBoxUsuario.Text;
                        Form2 form2 = new Form2(Jugador);
                        form2.Show();

                    }


                    else if (mensaje == "INCORRECTO")// nom contra incorrecte
                        MessageBox.Show("No se encuentra al usuario/contraseña incorrecta");

                    else if (mensaje == "ERROR")// ERROR
                        MessageBox.Show("ERROR");

                    // Se terminó el servicio. 
                    // Nos desconectamos
                    this.BackColor = Color.Gray;
                    server.Shutdown(SocketShutdown.Both);
                    server.Close();
                    
                }
                 

            }
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }


        }

        private void buttonRegistrarte_Click(object sender, EventArgs e)
        {
            try
            {
                if (String.IsNullOrEmpty(textBoxUsuario.Text) || String.IsNullOrEmpty(textBoxContraseña.Text)) // nome she ficat nom o contra  avisame
                {
                    MessageBox.Show("Escribe un nombre y una contraseña");
                }
                else
                {


                    // Creamos un IPEndPoint con el ip del servidor y puerto del sevidor al que conectamos
                    IPAddress direc = IPAddress.Parse("192.168.56.102");
                    IPEndPoint ipep = new IPEndPoint(direc, port);

                    //Creamos el socket 
                    server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                    server.Connect(ipep);//Intentamos conectar el socket
                    MessageBox.Show("Conectado");

                    string mensaje = "2/" + textBoxUsuario.Text + "/" + textBoxContraseña.Text;
                    // Enviamos al servidor el usuario y la contraseña
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];


                    if (mensaje == "BIEN") //ha anat be obram un form per fer peticions
                        MessageBox.Show("Se ha registrado");

                    else if (mensaje == "EXISTE") //si el usuario ya existe
                        MessageBox.Show("Este usuario ya existe");

                    else if (mensaje == "ERROR")// ERROR
                        MessageBox.Show("ERROR");
                   
                    // Se terminó el servicio. 
                    // Nos desconectamos
                    this.BackColor = Color.Gray;
                    server.Shutdown(SocketShutdown.Both);
                    server.Close();
                }
                
            }
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }         
        }

        private void buttonConsulta1_Click(object sender, EventArgs e)
        {
            try
            {

                // Creamos un IPEndPoint con el ip del servidor y puerto del sevidor al que conectamos
                IPAddress direc = IPAddress.Parse("192.168.56.102");
                IPEndPoint ipep = new IPEndPoint(direc, port);

                //Creamos el socket 
                server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                server.Connect(ipep);//Intentamos conectar el socket
                MessageBox.Show("Conectado");

                string mensaje = "3/";
                // Enviamos al servidor la consulta
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];

                MessageBox.Show(mensaje);

                // Se terminó el servicio. 
                // Nos desconectamos
                this.BackColor = Color.Gray;
                server.Shutdown(SocketShutdown.Both);
                server.Close();

                }
                catch (SocketException ex)
                {
                    //Si hay excepcion imprimimos error y salimos del programa con return 
                    MessageBox.Show("No he podido conectar con el servidor");
                    return;
                } 
        }

        private void buttonConsulta2_Click(object sender, EventArgs e)
        {
            try
            {
                if (String.IsNullOrEmpty(textBoxUsuario.Text)) // si no he puesto un nombre avisame
                {
                    MessageBox.Show("Escribe un nombre");
                }
                else
                {


                    // Creamos un IPEndPoint con el ip del servidor y puerto del sevidor al que conectamos
                    IPAddress direc = IPAddress.Parse("192.168.56.102");
                    IPEndPoint ipep = new IPEndPoint(direc, port);

                    //Creamos el socket 
                    server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                    server.Connect(ipep);//Intentamos conectar el socket
                    MessageBox.Show("Conectado");

                    string mensaje = "4/" + textBoxUsuario.Text;
                    // Enviamos al servidor el usuario y la contraseña
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];


                    if (mensaje == "SI") //ha anat be obram un form per fer peticions
                        MessageBox.Show("Se exsiste");

                    else if (mensaje == "NO") //si el usuario ya existe
                        MessageBox.Show("No exsiste");

                    else if (mensaje == "ERROR")// ERROR
                        MessageBox.Show("ERROR");

                    // Se terminó el servicio. 
                    // Nos desconectamos
                    this.BackColor = Color.Gray;
                    server.Shutdown(SocketShutdown.Both);
                    server.Close();
                }

            }
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }
        }
    }
}
