"Hello, world!"
console.log("keilingnica hello world log");
var a = new Canvas(3, 1);
// console.log(a.width, " ", a.height);
// var b = a.getContext()

var vertices = [
    -0.5,0.5,0.0,
    -0.5,-0.5,0.0,
    0.5,-0.5,0.0, 
];

var farray = new Float32Array(vertices)
console.log(farray)

var gl = a.getContext()         

var vertex_buffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);

// Create a vertex shader object
var vertShader = gl.createShader(gl.VERTEX_SHADER);

// bindTest.test()
// console.log(bindTest.testwithreturn())
// console.log(bindTest.testwithparam(1, 1))

// var b = new DynamicBindTest();
// b.test()
// console.log(b.testwithreturn())
// console.log(b.testwithparam(1, 1))
// console.log(b.width)