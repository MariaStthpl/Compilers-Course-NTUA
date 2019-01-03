; ModuleID = 'alan program'

@vars = private global [26 x i32] zeroinitializer, align 16
@nl = private constant [2 x i8] c"\0A\00", align 1

declare void @writeInteger(i16)

declare void @writeChar(i8)

declare void @writeString(i8*)

declare i8 @readInteger()

define void @main() {
entry:
  %a = alloca [3 x i16]
  %g = alloca [10 x i8]
  %y = alloca i8
  %x = alloca i16
  store i8 114, i8* %y
  %g1 = getelementptr [10 x i8], [10 x i8]* %g, i16 0, i32 0
  store i8 101, i8* %g1
  %g2 = load [10 x i8], [10 x i8]* %g
  %g3 = getelementptr inbounds [10 x i8], [10 x i8]* %g, i16 0, i16 0
  %g4 = load i8, i8* %g3
  %ext = zext i8 %g4 to i16
  call void @writeInteger(i16 %ext)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  call void @writeInteger(i16 97)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  %y5 = load i8, i8* %y
  %ext6 = zext i8 %y5 to i16
  call void @writeInteger(i16 %ext6)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  %calltmp = call i8 @hehe(i16 1)
  %ext7 = zext i8 %calltmp to i16
  call void @writeInteger(i16 %ext7)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  ret void
}

define i8 @hehe(i16 %l) {
entry:
  %l1 = alloca i16
  store i16 %l, i16* %l1
  ret i8 104
}
