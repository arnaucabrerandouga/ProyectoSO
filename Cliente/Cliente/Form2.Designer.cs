namespace WindowsFormsApplication1
{
    partial class Form2
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonConsulta1 = new System.Windows.Forms.Button();
            this.buttonConsulta2 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // buttonConsulta1
            // 
            this.buttonConsulta1.Location = new System.Drawing.Point(35, 59);
            this.buttonConsulta1.Name = "buttonConsulta1";
            this.buttonConsulta1.Size = new System.Drawing.Size(221, 50);
            this.buttonConsulta1.TabIndex = 9;
            this.buttonConsulta1.Text = "Dime cuantos usuarios hay";
            this.buttonConsulta1.UseVisualStyleBackColor = true;
            this.buttonConsulta1.Click += new System.EventHandler(this.buttonConsulta1_Click);
            // 
            // buttonConsulta2
            // 
            this.buttonConsulta2.Location = new System.Drawing.Point(35, 165);
            this.buttonConsulta2.Name = "buttonConsulta2";
            this.buttonConsulta2.Size = new System.Drawing.Size(221, 50);
            this.buttonConsulta2.TabIndex = 10;
            this.buttonConsulta2.Text = "Dime si este usuario existe?";
            this.buttonConsulta2.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(385, 74);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(51, 20);
            this.label1.TabIndex = 11;
            this.label1.Text = "label1";
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(630, 541);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonConsulta2);
            this.Controls.Add(this.buttonConsulta1);
            this.Name = "Form2";
            this.Text = "Form2";
            this.Load += new System.EventHandler(this.Form2_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonConsulta1;
        private System.Windows.Forms.Button buttonConsulta2;
        private System.Windows.Forms.Label label1;
    }
}