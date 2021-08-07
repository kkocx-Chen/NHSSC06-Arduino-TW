<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form 覆寫 Dispose 以清除元件清單。
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    '為 Windows Form 設計工具的必要項
    Private components As System.ComponentModel.IContainer

    '注意: 以下為 Windows Form 設計工具所需的程序
    '可以使用 Windows Form 設計工具進行修改。
    '請勿使用程式碼編輯器進行修改。
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.StartButton = New System.Windows.Forms.Button()
        Me.StopButton = New System.Windows.Forms.Button()
        Me.SerialPort1 = New System.IO.Ports.SerialPort(Me.components)
        Me.CleanButton = New System.Windows.Forms.Button()
        Me.RichTextBox1 = New System.Windows.Forms.RichTextBox()
        Me.SuspendLayout()
        '
        'StartButton
        '
        Me.StartButton.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.StartButton.Font = New System.Drawing.Font("新細明體", 20.0!)
        Me.StartButton.Location = New System.Drawing.Point(97, 176)
        Me.StartButton.Name = "StartButton"
        Me.StartButton.Size = New System.Drawing.Size(262, 133)
        Me.StartButton.TabIndex = 0
        Me.StartButton.Text = "Start"
        Me.StartButton.UseVisualStyleBackColor = True
        '
        'StopButton
        '
        Me.StopButton.Font = New System.Drawing.Font("新細明體", 20.0!)
        Me.StopButton.Location = New System.Drawing.Point(434, 176)
        Me.StopButton.Name = "StopButton"
        Me.StopButton.Size = New System.Drawing.Size(262, 133)
        Me.StopButton.TabIndex = 1
        Me.StopButton.Text = "Stop"
        Me.StopButton.UseVisualStyleBackColor = True
        '
        'SerialPort1
        '
        Me.SerialPort1.PortName = "COM2"
        '
        'CleanButton
        '
        Me.CleanButton.Font = New System.Drawing.Font("新細明體", 20.0!)
        Me.CleanButton.Location = New System.Drawing.Point(771, 176)
        Me.CleanButton.Name = "CleanButton"
        Me.CleanButton.Size = New System.Drawing.Size(256, 133)
        Me.CleanButton.TabIndex = 2
        Me.CleanButton.Text = "Clean"
        Me.CleanButton.UseVisualStyleBackColor = True
        '
        'RichTextBox1
        '
        Me.RichTextBox1.Font = New System.Drawing.Font("新細明體", 25.0!)
        Me.RichTextBox1.Location = New System.Drawing.Point(434, 333)
        Me.RichTextBox1.Name = "RichTextBox1"
        Me.RichTextBox1.Size = New System.Drawing.Size(244, 285)
        Me.RichTextBox1.TabIndex = 3
        Me.RichTextBox1.Text = ""
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1180, 660)
        Me.Controls.Add(Me.RichTextBox1)
        Me.Controls.Add(Me.CleanButton)
        Me.Controls.Add(Me.StopButton)
        Me.Controls.Add(Me.StartButton)
        Me.Name = "Form1"
        Me.Text = "Form1"
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents StartButton As Button
    Friend WithEvents StopButton As Button
    Friend WithEvents SerialPort1 As IO.Ports.SerialPort
    Friend WithEvents CleanButton As Button
    Friend WithEvents RichTextBox1 As RichTextBox
End Class
