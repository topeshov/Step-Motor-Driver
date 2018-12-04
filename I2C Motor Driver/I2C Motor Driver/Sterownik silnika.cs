using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MCP2221;

namespace I2C_Motor_Driver
{
    public partial class Form1 : Form
    {
        MCP2221.MchpUsbI2c UsbI2c = new MchpUsbI2c();
        bool isConnected;
        byte[] Stop = { 0 };
        byte[] Direction = { 255 };
        byte Adres = 2;
        int isNumber = 0;


        public Form1()
        {
            InitializeComponent();
            button1.Enabled = false;
            button2.Enabled = false;
            button3.Enabled = false;
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void Szukaj_Click(object sender, EventArgs e)
        {
            string Connected;
            
            isConnected = UsbI2c.Settings.GetConnectionStatus();
            
            if (isConnected == true)
            {
                button1.Enabled = true;
                button2.Enabled = true;
                button3.Enabled = true;

                Connected = "Układ MCP2221 został podłączony.\n";
                
                int devCount = UsbI2c.Management.GetDevCount();
                string AmountMCP = "Podłączony jest " + devCount.ToString() + " układ MCP2221 do komputera PC.\n";
                UsbI2c.Management.SelectDev(0);
                
                string usbDescriptor = UsbI2c.Settings.GetUsbStringDescriptor();
                string Descriptor = "Deskryptor USB: " + usbDescriptor + "\n";
                
                richTextBox1.Text = Connected + AmountMCP + Descriptor;
            }
            else
            {
                button1.Enabled = false;
                button2.Enabled = false;
                button3.Enabled = false;
                Connected = "Układ MCP2221 jest niepodłączony.\n";
                richTextBox1.Text = Connected;
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (UsbI2c.Functions.WriteI2cData(Adres, Stop, 1, 9600) == 0)
                richTextBox1.Text += "OK - Stan zmieniony\n";
            else
                richTextBox1.Text += "Błąd połączenia\n";
            UsbI2c.Functions.StopI2cDataTransfer();
        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (UsbI2c.Functions.WriteI2cData(Adres, Direction, 1, 9600) == 0)
                richTextBox1.Text += "OK - Kierunek zmieniony\n";
            else
                richTextBox1.Text += "Błąd połączenia\n";
            UsbI2c.Functions.StopI2cDataTransfer();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            int InputRot = Int32.Parse(textBox1.Text);
            if (InputRot > 0 && InputRot < 255)
            {
                byte RotationByte = Convert.ToByte(InputRot);
                byte[] Rotation = { RotationByte };
                if (UsbI2c.Functions.WriteI2cData(Adres, Rotation, 1, 9600) == 0)
                {
                    UsbI2c.Functions.StopI2cDataTransfer();
                    richTextBox1.Text += "Ustawiono liczbę obrotów: " + InputRot + "\n";
                }
                else
                    richTextBox1.Text += "Błąd połączenia\n";
            }
            else
                MessageBox.Show("Nieprawidłowa ilość obrotów!", "Błąd!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {

        }
    }
}

