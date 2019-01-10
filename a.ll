; ModuleID = 'alan program'

@string_literal = internal global [2 x i8] c"\0A\00"
@string_literal.1 = internal global [2 x i8] c"\0A\00"
@string_literal.2 = internal global [2 x i8] c"\0A\00"

declare void @writeInteger(i16)

define void @writeByte(i8) {
  %2 = sext i8 %0 to i16
  call void @writeInteger(i16 %2)
  ret void
}

declare void @writeChar(i8)

declare void @writeString(i8*)

declare i16 @readInteger()

declare i8 @readByte()

declare i8 @readChar()

declare void @readString(i16, i8*)

define i16 @extend(i8) {
  %2 = sext i8 %0 to i16
  ret i16 %2
}

define i8 @shrink(i16) {
  %2 = trunc i16 %0 to i8
  ret i8 %2
}

declare i16 @strlen(i8*)

declare i16 @strcmp(i8*, i8*)

declare void @strcpy(i8*, i8*)

declare void @strcat(i8*, i8*)

define void @main() {
entry:
  %y = alloca [10 x i16]
  %x = alloca [10 x i16]
  %a = alloca [2 x i16]
  %x1 = getelementptr inbounds [10 x i16], [10 x i16]* %x, i8 0, i8 0
  %y2 = getelementptr inbounds [10 x i16], [10 x i16]* %y, i8 0, i8 0
  %a3 = getelementptr inbounds [2 x i16], [2 x i16]* %a, i16 0
  %x4 = getelementptr inbounds [10 x i16], [10 x i16]* %x, i16 0
  %y5 = getelementptr inbounds [10 x i16], [10 x i16]* %y, i16 0
  call void @fun(i16* %x1, i16* %y2, [2 x i16]* %a3, [10 x i16]* %x4, [10 x i16]* %y5)
  %a6 = getelementptr inbounds [2 x i16], [2 x i16]* %a, i32 0, i16 100
  store i16 1, i16* %a6
  call void @writeInteger(i16 1)
  %str_ptr = getelementptr inbounds [2 x i8], [2 x i8]* @string_literal.2, i32 0, i32 0
  call void @writeString(i8* %str_ptr)
  ret void
}

define void @fun(i16* %x, i16* %y, [2 x i16]* %a, [10 x i16]* %x1, [10 x i16]* %y2) {
entry:
  %y27 = alloca [10 x i16]*
  %x16 = alloca [10 x i16]*
  %a5 = alloca [2 x i16]*
  %y4 = alloca i16*
  %x3 = alloca i16*
  store i16* %x, i16** %x3
  store i16* %y, i16** %y4
  store [2 x i16]* %a, [2 x i16]** %a5
  store [10 x i16]* %x1, [10 x i16]** %x16
  store [10 x i16]* %y2, [10 x i16]** %y27
  %x8 = load i16*, i16** %x3
  %x9 = getelementptr inbounds i16, i16* %x8, i16 0
  store i16 1, i16* %x9
  %x10 = load i16*, i16** %x3
  %x11 = getelementptr inbounds i16, i16* %x10, i16 1
  store i16 1, i16* %x11
  %y12 = load i16*, i16** %y4
  %y13 = getelementptr inbounds i16, i16* %y12, i16 0
  store i16 2, i16* %y13
  %y14 = load i16*, i16** %y4
  %y15 = getelementptr inbounds i16, i16* %y14, i16 1
  store i16 2, i16* %y15
  %y16 = load i16*, i16** %y4
  %y17 = getelementptr inbounds i16, i16* %y16, i16 2
  store i16 2, i16* %y17
  %x18 = load i16*, i16** %x3
  %x19 = getelementptr inbounds i16, i16* %x18, i16 0
  %var = load i16, i16* %x19
  call void @writeInteger(i16 %var)
  %x20 = load i16*, i16** %x3
  %x21 = getelementptr inbounds i16, i16* %x20, i16 1
  %var22 = load i16, i16* %x21
  call void @writeInteger(i16 %var22)
  %str_ptr = getelementptr inbounds [2 x i8], [2 x i8]* @string_literal, i32 0, i32 0
  call void @writeString(i8* %str_ptr)
  %y23 = load i16*, i16** %y4
  %y24 = getelementptr inbounds i16, i16* %y23, i16 0
  %var25 = load i16, i16* %y24
  call void @writeInteger(i16 %var25)
  %str_ptr26 = getelementptr inbounds [2 x i8], [2 x i8]* @string_literal.1, i32 0, i32 0
  call void @writeString(i8* %str_ptr26)
  ret void
}
