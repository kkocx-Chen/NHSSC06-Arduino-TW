Module EasyTool

    Public Function GetSerialsNames() '讀取串口陣列
        Dim portsName(99) As String
        Dim i As Integer = 0
        For Each sp As String In My.Computer.Ports.SerialPortNames '讀取電腦所連結的COM節點 並加入倒 Combobox裡面
            portsName(i) = sp
        Next
        Return portsName
    End Function

    Public Sub UpdateSerialPorts(ComPorts) '更新串口列表
        For Each SerialPortName As String In GetSerialsNames()
            If SerialPortName <> "" Then
                ComPorts.Items.Add(SerialPortName)
            End If
        Next
    End Sub

    Public Sub UpdateTimer(TextBox1)  '讀取時間
        TextBox1.Text = "Current Time: " & DateTime.Now.ToString("下午HH:mm:ss")
    End Sub

    Public Function ButtonCheck(checkbutton) '偵測按鈕有無按下
        If checkbutton.Focused Then
            Return False
        Else
            Return True
        End If
    End Function
    Public Function IsPasswordCorrect(password) '藍芽需要密碼才能點亮LED
        If password = "123456" Then
            Return True
        Else
            Return False
        End If
    End Function

    Public Sub ChangeMultiCheckBoxEnable(Form As Form, TargetStr As String, Enable As Boolean)
        For Each cntrl As Control In Form.Controls
            If InStr(cntrl.Name, TargetStr) > 0 Then
                cntrl.Enabled = Enable
            End If
        Next
    End Sub
    Public Sub SendCommand(serial As IO.Ports.SerialPort, command As Integer)
        serial.Write(command)
    End Sub
    Public Function ReadCommand(serial As IO.Ports.SerialPort)
        Dim command(1) As Byte
        serial.Read(command, 0, 1)
        Return command
    End Function
End Module
