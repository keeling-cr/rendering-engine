"Hello, world!"
console.log("keilingnica hello world log");
var a = new Canvas(3, 1);
// console.log(a.width, " ", a.height);
// var b = a.getContext()

var gl = a.getContext()
var buffer = gl.createBuffer()
gl.bindBuffer(gl.ARRAY_BUFFER, buffer)
gl.clearColor(0.0, 1.0, 1.0, 1.0);

// bindTest.test()
// console.log(bindTest.testwithreturn())
// console.log(bindTest.testwithparam(1, 1))

// var b = new DynamicBindTest();
// b.test()
// console.log(b.testwithreturn())
// console.log(b.testwithparam(1, 1))
// console.log(b.width)