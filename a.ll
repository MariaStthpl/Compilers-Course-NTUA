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
  %i = alloca i16
  %a = alloca [10 x i16]
  store i16 0, i16* %i
  %i1 = load i16, i16* %i
  %ltmp = icmp slt i16 %i1, i16 10
  %while_entry = icmp ne i1 %ltmp, i32 0
  br i1 %while_entry, label %while, label %while_end
  %a5 = getelementptr [10 x i16], [10 x i16]* %a, i16 0, i32 %ext
  %a12 = getelementptr inbounds [10 x i16], [10 x i16]* %a, i16 0, i16 6
  %str = getelementptr inbounds [3 x i8], [3 x i8]* %string, i8 0, i8 0
  %str14 = getelementptr inbounds [3 x i8], [3 x i8]* %string, i8 0, i8 1
  %str15 = getelementptr inbounds [3 x i8], [3 x i8]* %string, i8 0, i8 2
  %str_ptr = getelementptr inbounds [3 x i8], [3 x i8]* %string, i8 0, i8 0
  ret void

while:                                            ; preds = %while, %entry
  %i2 = load i16, i16* %i
  %i3 = load i16, i16* %i
  %addtmp = add i16 %i3, i16 1
  %multmp = mul i16 %i2, %addtmp
  %i4 = load i16, i16* %i
  %ext = sext i16 %i4 to i32
  store i16 %multmp, i16* %a5
  %i6 = load i16, i16* %i
  %addtmp7 = add i16 %i6, i16 1
  store i16 %addtmp7, i16* %i
  %i8 = load i16, i16* %i
  %ltmp9 = icmp slt i16 %i8, i16 10
  %while_entry10 = icmp ne i1 %ltmp9, i32 0
  br i1 %while_entry10, label %while, label %while_end

while_end:                                        ; preds = %while, %entry
  %a11 = load [10 x i16], [10 x i16]* %a
  %a13 = load i16, i16* %a12
  call void @writeInteger(i16 %a13)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  %string = alloca [3 x i8]
  store i8 92, i8* %str
  store i8 110, i8* %str14
  store i8 0, i8* %str15
  call void @writeString(i8* %str_ptr)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
