; ModuleID = 'alan program'

@vars = private global [26 x i32] zeroinitializer, align 16
@nl = private constant [2 x i8] c"\0A\00", align 1

declare void @writeInteger(i16)

declare i8 @ord(i8)

declare void @writeChar(i8)

declare void @writeString(i8*)

declare i16 @readInteger()

declare i8 @readChar()

declare void @readString(i16, i8*)

declare i16 @strlen(i8*)

declare i16 @strcmp(i8*, i8*)

declare void @strcpy(i8*, i8*)

declare void @strcat(i8*, i8*)

define void @main() {
entry:
  %e = alloca [100 x i8]
  %z = alloca i16
  %y = alloca i8
  %x = alloca i16
  %c = alloca [100 x i8]
  %b = alloca [100 x i16]
  %a = alloca [100 x i16]
  %c1 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i8 0, i8 0
  %string = alloca [8 x i8]
  %str = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 0
  store i8 97, i8* %str
  %str2 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 1
  store i8 100, i8* %str2
  %str3 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 2
  store i8 97, i8* %str3
  %str4 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 3
  store i8 115, i8* %str4
  %str5 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 4
  store i8 102, i8* %str5
  %str6 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 5
  store i8 97, i8* %str6
  %str7 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 6
  store i8 100, i8* %str7
  %str8 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 7
  store i8 0, i8* %str8
  %str_ptr = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 0
  %string9 = alloca [8 x i8]
  %str10 = getelementptr inbounds [8 x i8], [8 x i8]* %string9, i8 0, i8 0
  store i8 97, i8* %str10
  %str11 = getelementptr inbounds [8 x i8], [8 x i8]* %string9, i8 0, i8 1
  store i8 100, i8* %str11
  %str12 = getelementptr inbounds [8 x i8], [8 x i8]* %string9, i8 0, i8 2
  store i8 97, i8* %str12
  %str13 = getelementptr inbounds [8 x i8], [8 x i8]* %string9, i8 0, i8 3
  store i8 115, i8* %str13
  %str14 = getelementptr inbounds [8 x i8], [8 x i8]* %string9, i8 0, i8 4
  store i8 102, i8* %str14
  %str15 = getelementptr inbounds [8 x i8], [8 x i8]* %string9, i8 0, i8 5
  store i8 97, i8* %str15
  %str16 = getelementptr inbounds [8 x i8], [8 x i8]* %string9, i8 0, i8 6
  store i8 100, i8* %str16
  %str17 = getelementptr inbounds [8 x i8], [8 x i8]* %string9, i8 0, i8 7
  store i8 0, i8* %str17
  %str_ptr18 = getelementptr inbounds [8 x i8], [8 x i8]* %string9, i8 0, i8 0
  %c19 = load [100 x i8], [100 x i8]* %c
  %c20 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i16 0, i16 0
  %c21 = load i8, i8* %c20
  %c22 = getelementptr [100 x i8], [100 x i8]* %c, i16 0, i32 0
  store i8 97, i8* %c22
  %c23 = load [100 x i8], [100 x i8]* %c
  %c24 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i16 0, i16 1
  %c25 = load i8, i8* %c24
  %c26 = getelementptr [100 x i8], [100 x i8]* %c, i16 0, i32 1
  store i8 100, i8* %c26
  %c27 = load [100 x i8], [100 x i8]* %c
  %c28 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i16 0, i16 2
  %c29 = load i8, i8* %c28
  %c30 = getelementptr [100 x i8], [100 x i8]* %c, i16 0, i32 2
  store i8 97, i8* %c30
  %c31 = load [100 x i8], [100 x i8]* %c
  %c32 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i16 0, i16 3
  %c33 = load i8, i8* %c32
  %c34 = getelementptr [100 x i8], [100 x i8]* %c, i16 0, i32 3
  store i8 115, i8* %c34
  %c35 = load [100 x i8], [100 x i8]* %c
  %c36 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i16 0, i16 4
  %c37 = load i8, i8* %c36
  %c38 = getelementptr [100 x i8], [100 x i8]* %c, i16 0, i32 4
  store i8 102, i8* %c38
  %c39 = load [100 x i8], [100 x i8]* %c
  %c40 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i16 0, i16 5
  %c41 = load i8, i8* %c40
  %c42 = getelementptr [100 x i8], [100 x i8]* %c, i16 0, i32 5
  store i8 97, i8* %c42
  %c43 = load [100 x i8], [100 x i8]* %c
  %c44 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i16 0, i16 6
  %c45 = load i8, i8* %c44
  %c46 = getelementptr [100 x i8], [100 x i8]* %c, i16 0, i32 6
  store i8 100, i8* %c46
  %c47 = load [100 x i8], [100 x i8]* %c
  %c48 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i16 0, i16 7
  %c49 = load i8, i8* %c48
  %c50 = getelementptr [100 x i8], [100 x i8]* %c, i16 0, i32 7
  store i8 0, i8* %c50
  %a51 = load [100 x i16], [100 x i16]* %a
  %a52 = getelementptr inbounds [100 x i16], [100 x i16]* %a, i16 0, i16 1000
  %a53 = load i16, i16* %a52
  %a54 = getelementptr [100 x i16], [100 x i16]* %a, i16 0, i32 1000
  store i16 3, i16* %a54
  %a55 = load [100 x i16], [100 x i16]* %a
  %a56 = getelementptr inbounds [100 x i16], [100 x i16]* %a, i16 0, i16 0
  %a57 = load i16, i16* %a56
  %a58 = getelementptr [100 x i16], [100 x i16]* %a, i16 0, i32 0
  store i16 1, i16* %a58
  %a59 = load [100 x i16], [100 x i16]* %a
  %a60 = getelementptr inbounds [100 x i16], [100 x i16]* %a, i16 0, i16 0
  %a61 = load i16, i16* %a60
  call void @writeInteger(i16 %a61)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  %a62 = getelementptr inbounds [100 x i16], [100 x i16]* %a, i16 0
  %b63 = getelementptr inbounds [100 x i16], [100 x i16]* %b, i16 0
  %c64 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i16 0
  %e65 = getelementptr inbounds [100 x i8], [100 x i8]* %e, i16 0
  %x66 = getelementptr inbounds i16, i16* %x, i16 0
  %y67 = getelementptr inbounds i8, i8* %y, i16 0
  %z68 = getelementptr inbounds i16, i16* %z, i16 0
  %calltmp = call i16 @hehe([100 x i16]* %a62, [100 x i16]* %b63, [100 x i8]* %c64, [100 x i8]* %e65, i16* %x66, i8* %y67, i16* %z68)
  %x69 = load i16, i16* %x
  call void @writeInteger(i16 %x69)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  ret void
}

define i16 @hehe([100 x i16]* %a, [100 x i16]* %b, [100 x i8]* %c, [100 x i8]* %e, i16* %x, i8* %y, i16* %z) {
entry:
  %z7 = alloca i16*
  %y6 = alloca i8*
  %x5 = alloca i16*
  %e4 = alloca [100 x i8]*
  %c3 = alloca [100 x i8]*
  %b2 = alloca [100 x i16]*
  %a1 = alloca [100 x i16]*
  store [100 x i16]* %a, [100 x i16]** %a1
  store [100 x i16]* %b, [100 x i16]** %b2
  store [100 x i8]* %c, [100 x i8]** %c3
  store [100 x i8]* %e, [100 x i8]** %e4
  store i16* %x, i16** %x5
  store i8* %y, i8** %y6
  store i16* %z, i16** %z7
  %a8 = load [100 x i16]*, [100 x i16]** %a1
  %a9 = getelementptr inbounds [100 x i16], [100 x i16]* %a8, i16 10
  %a10 = load [100 x i16], [100 x i16]* %a9
  %a11 = load [100 x i16]*, [100 x i16]** %a1
  %a12 = getelementptr [100 x i16], [100 x i16]* %a11, i32 10
  store i16 2, [100 x i16]* %a12
  %x13 = load i16*, i16** %x5
  %x14 = getelementptr inbounds i16, i16* %x13, i16 0
  %x15 = load i16*, i16** %x5
  %x16 = getelementptr inbounds i16, i16* %x15, i16 0
  %x17 = load i16*, i16** %x5
  %x18 = getelementptr inbounds i16, i16* %x17, i16 0
  %x19 = load i16*, i16** %x5
  %x20 = getelementptr inbounds i16, i16* %x19, i16 0
  store i16* %x18, i16* %x20
  %a21 = load [100 x i16]*, [100 x i16]** %a1
  %a22 = getelementptr inbounds [100 x i16], [100 x i16]* %a21, i16 0
  %a23 = load [100 x i16], [100 x i16]* %a22
  call void @writeInteger([100 x i16] %a23)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  ret i16 5
}
