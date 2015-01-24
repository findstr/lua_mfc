--MsgBox("Hello", "World")

id1 = CreateStatic("Hello", 10, 10, 50, 20)
--MsgBox(id, "ID")

AddString(id1, "B1")

str = GetString(id1);
--MgBox(str, "ID")

id = CreateComboBox(100, 10, 200, 20)
--MsgBox(id, "ID")
--
AddString(id, "A1")
AddString(id, "A2")
AddString(id, "A3")
AddString(id, "A1")


function OnMsg(aa, msg)
	t1 = GetString(aa)
	AddString(id1, t1)
end

