Imports _109vb.EasyTool
Public Class Form1
    Private Sub StartButton_Click(sender As Object, e As EventArgs) Handles StartButton.Click
        SendCommand(SerialPort1, 1)

    End Sub

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        SerialPort1.Open()
    End Sub

    Private Sub StopButton_Click(sender As Object, e As EventArgs) Handles StopButton.Click
        SendCommand(SerialPort1, 2)
    End Sub

    Private Sub CleanButton_Click(sender As Object, e As EventArgs) Handles CleanButton.Click
        SendCommand(SerialPort1, 3)
    End Sub

    Private Sub RichTextBox1_TextChanged(sender As Object, e As EventArgs) Handles RichTextBox1.TextChanged
        ReadCommand(SerialPort1)
    End Sub
End Class
