
namespace Bootloader_Form2020
{
    partial class Bootloader
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.Select_HEX_File = new System.Windows.Forms.Button();
            this.Envoi = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.File_Name = new System.Windows.Forms.Label();
            this.Avancement = new System.Windows.Forms.Label();
            this.Destination = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // Select_HEX_File
            // 
            this.Select_HEX_File.Location = new System.Drawing.Point(13, 13);
            this.Select_HEX_File.Name = "Select_HEX_File";
            this.Select_HEX_File.Size = new System.Drawing.Size(75, 23);
            this.Select_HEX_File.TabIndex = 0;
            this.Select_HEX_File.Text = "Select .HEX";
            this.Select_HEX_File.UseVisualStyleBackColor = true;
            this.Select_HEX_File.Click += new System.EventHandler(this.Select_HEX_File_Click);
            // 
            // Envoi
            // 
            this.Envoi.Location = new System.Drawing.Point(12, 70);
            this.Envoi.Name = "Envoi";
            this.Envoi.Size = new System.Drawing.Size(75, 23);
            this.Envoi.TabIndex = 1;
            this.Envoi.Text = "Burn";
            this.Envoi.UseVisualStyleBackColor = true;
            this.Envoi.Click += new System.EventHandler(this.Envoi_Click);
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 1000000;
            this.serialPort1.PortName = "COM5";
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(12, 100);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(221, 23);
            this.progressBar1.TabIndex = 2;
            // 
            // File_Name
            // 
            this.File_Name.AutoSize = true;
            this.File_Name.Location = new System.Drawing.Point(94, 18);
            this.File_Name.Name = "File_Name";
            this.File_Name.Size = new System.Drawing.Size(16, 13);
            this.File_Name.TabIndex = 3;
            this.File_Name.Text = "...";
            // 
            // Avancement
            // 
            this.Avancement.AutoSize = true;
            this.Avancement.Location = new System.Drawing.Point(239, 105);
            this.Avancement.Name = "Avancement";
            this.Avancement.Size = new System.Drawing.Size(0, 13);
            this.Avancement.TabIndex = 4;
            // 
            // Destination
            // 
            this.Destination.FormattingEnabled = true;
            this.Destination.Items.AddRange(new object[] {
            "IA",
            "MultiFct_1",
            "MultiFct_2",
            "MultiFct_3",
            "MultiFct_4"});
            this.Destination.Location = new System.Drawing.Point(13, 43);
            this.Destination.Name = "Destination";
            this.Destination.Size = new System.Drawing.Size(121, 21);
            this.Destination.TabIndex = 5;
            // 
            // Bootloader
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.Destination);
            this.Controls.Add(this.Avancement);
            this.Controls.Add(this.File_Name);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.Envoi);
            this.Controls.Add(this.Select_HEX_File);
            this.Name = "Bootloader";
            this.Text = "Robotic System: Bootloader";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Select_HEX_File;
        private System.Windows.Forms.Button Envoi;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Label File_Name;
        private System.Windows.Forms.Label Avancement;
        private System.Windows.Forms.ComboBox Destination;
    }
}

