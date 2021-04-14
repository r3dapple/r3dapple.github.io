/*
* Copyright 2021 Markus Heimerl, OTH Regensburg, r3dapple.de
* Licensed under CC BY-SA 4.0
* r3dapple.de/LICENSE.txt
*
* ANY USE OF THIS SOFTWARE MUST COMPLY WITH THE
* CREATIVE COMMONS ATTRIBUTION-SHAREALIKE 4.0 INTERNATIONAL LICENSE
* AGREEMENTS
*/

cellularautomata3d();
function cellularautomata3d(){

	
	var cellgrid = [][][];
	cellularAutomataLogic();
	function cellularAutomataLogic(){
		if(running){}
		
	}



	// --- DRAWING CODE ---
	function parseOBJ(data){

		var allVerticies = [];
		var allTexCoords = [];
		var allNormals = [];


		var objects = {};
		var objname = "";

		var individualLines = data.split('\n');
		
		for(var i = 0; i < individualLines.length; i++){
			var lineElements = individualLines[i].split(' ');
			var type = lineElements[0];
			switch(type){
				case "#":
					break;
				case " ":
					break;
				case "o":
					objname = lineElements[1];
					objects[objname] = {};
					objects[objname].positions = [];
					objects[objname].texcoords = [];
					objects[objname].normals = [];
					break;
				case "v":
					allVerticies.push([lineElements[1], lineElements[2], lineElements[3]].map(parseFloat));
					break;
				case "vt":
					allTexCoords.push([lineElements[1], lineElements[2]].map(parseFloat));
					break;
				case "vn":
					allNormals.push([lineElements[1], lineElements[2], lineElements[3]].map(parseFloat));
					break;
				case "f":
					var faceParts = [];
					for(var j = 0; j < lineElements.length - 1; j++){ 
						faceParts[j] = lineElements[j+1].split('/').map(str => parseInt(str)-1);
					}
					objects[objname].positions.push(allVerticies[faceParts[0][0]], allVerticies[faceParts[1][0]], allVerticies[faceParts[2][0]]);
					objects[objname].texcoords.push(allTexCoords[faceParts[0][1]], allTexCoords[faceParts[1][1]], allTexCoords[faceParts[2][1]]);
					objects[objname].normals.push(allNormals[faceParts[0][2]], allNormals[faceParts[1][2]], allNormals[faceParts[2][2]]);
					break;
				default:
					//console.warn("objloader: unhandled keyword: ", type);
			}
		}

		for(var obj in objects){
			objects[obj].positions = objects[obj].positions.flat();
			objects[obj].texcoords = objects[obj].texcoords.flat();
			objects[obj].normals = objects[obj].normals.flat();
		}

		return objects;
	}


	const vertexshadersource = `
		precision highp float;

		attribute vec4 vertexposition;
		attribute vec2 texturecoordinate;
		attribute vec3 normal;

		uniform mat4 modelmatrix;
		uniform mat4 projectionmatrix;
		uniform mat4 viewmatrix;
		uniform mat4 inversetransposemodelmatrix;

		varying vec2 o_texturecoordinate;
		varying vec3 o_normal;

		void main(){
			o_texturecoordinate = texturecoordinate;
			o_normal = mat3(modelmatrix) * normal;
			gl_Position = projectionmatrix * viewmatrix * modelmatrix * vertexposition;
		}
	`;

	const fragmentshadersource = `
		precision highp float;

		varying vec2 o_texturecoordinate;
		varying vec3 o_normal;

		uniform sampler2D texture;
		uniform vec3 reverseLightDirection;

		void main(){
			vec3 normal = normalize(o_normal);
			float light = dot(normal, reverseLightDirection);

			gl_FragColor = texture2D(texture, o_texturecoordinate);
			//gl_FragColor.rgb *= light;
		}
	`;
	// --- GET CANVAS CONTEXT AND SETUP KEY LISTENERS ---
	var running = true;

	var fscanvas = $("#webglstudycanvas")[0];
	fscanvas.width = Math.pow(2, Math.floor(getBaseLog(2, $(window).width() * 0.88)));
	window.addEventListener("orientationchange", function() {
		setTimeout(function(){
			var newwidth = Math.pow(2, Math.floor(getBaseLog(2, $(window).width() * 0.88)));
			fscanvas.width = newwidth;
		}, 200);
	});
	var gl = fscanvas.getContext("webgl", {antialias: true});
	const r3webgl = {...parentr3webgl};
	r3webgl.gl = gl;
	gl.enable(gl.SAMPLE_COVERAGE);
	gl.sampleCoverage(0.5, false);

	$("#webglstudytoggle").click(toggle);
	function toggle(){running ? (running = false, $("#webglstudytoggle").html("Start")) : (running = true, $("#webglstudytoggle").html("Stop"));}
	// https://developer.mozilla.org/en-US/docs/Web/API/Pointer_Lock_API

	// --- ADD EVENT LISTENERS ---
	fscanvas.onclick = function(){
		fscanvas.requestPointerLock();
	}

	var viewxz = 0;
	var viewy = 0;

	document.addEventListener("pointerlockchange", function(){
		if (document.pointerLockElement === fscanvas) {
		  document.addEventListener("mousemove", updatePosition, false);
		} else {
		  document.removeEventListener("mousemove", updatePosition, false);
		}
	}, false);

	function updatePosition(e) {
		viewxz -= e.movementX*0.1;
		viewy = Math.min(90, Math.max(-90, viewy-e.movementY*0.1));
	}

	var keys = {};
	$("#webglstudycanvas").keydown(function(event) {
			keys[event.key] = true;
	});
	$("#webglstudycanvas").keyup(function(event) {
			keys[event.key] = false;
	});
	// --- ---

	// --- MAKE SHADERS AND PROGRAM ---
	const program = r3webgl.createShaderProgram(vertexshadersource, fragmentshadersource);
	gl.useProgram(program);

	// --- GET ALL ATTRIBUTE AND UNIFORM LOCATIONS
	const attribLocations = r3webgl.getAttribLocations(program, ["vertexposition", "texturecoordinate", "normal"]);
	const uniformLocations = r3webgl.getUniformLocations(program, ["modelmatrix", "viewmatrix", "projectionmatrix", "texture", "reverseLightDirection", "inversetransposemodelmatrix"]);

	// --- INIT 3D ---
	r3webgl.init3D();

	// --- MAKE BUFFER WITH POSITION DATA FOR LETTER F --- 
	var vertexbuffer = r3webgl.createBuffer(gl.ARRAY_BUFFER, vertex_data.positions);

	// --- NORMAL COORDINATES FOR EACH VERTEX ---
	var normalbuffer = r3webgl.createBuffer(gl.ARRAY_BUFFER, vertex_data.normals);


	// --- MAKE TEXTURE COORDINATE DATA ---
	for(var i = 0; i < vertex_data.texcoords.length; i++){
		// convert to texture clip space. Texture width and height is 256
		vertex_data.texcoords[i] = vertex_data.texcoords[i] / 256;
	}
	var texcoordbuffer = r3webgl.createBuffer(gl.ARRAY_BUFFER, vertex_data.texcoords);


	// --- CREATE TEXTURE AND GET RESOURCE ---
	var texture = r3webgl.createTexture();
	r3webgl.attachTextureSourceAsync(texture, "webgl3dstudy/f-texture.png", false);

	// --- THERE SHALL BE LIGHT ---
	gl.uniform3fv(uniformLocations.reverseLightDirection, m4.normalize([1.0, 0.0, 0.0, 1.0]));

	// GET DATA FROM OBJ
	var cubevertexbuffer;
	var cubetexcoordbuffer;
	var cubenormalbuffer;
	main();
	async function main() {
		const response = await fetch('webgl3dstudy/cube.obj');
		const text = await response.text();
		var data = parseOBJ(text, true);
		console.log(data);
		cubevertexbuffer  = r3webgl.createBuffer(gl.ARRAY_BUFFER, data["Cube"].positions);
		cubetexcoordbuffer = r3webgl.createBuffer(gl.ARRAY_BUFFER, data["Cube"].texcoords);
		cubenormalbuffer = r3webgl.createBuffer(gl.ARRAY_BUFFER, data["Cube"].normals);
	}
	// --- GET OBJ TEXTURE ---
	var texturecube = r3webgl.createTexture();
	r3webgl.attachTextureSourceAsync(texturecube, "webgl3dstudy/cubetexturetest.png", true);

	// --- ENABLE TEXTURE0 ---
	gl.uniform1i(uniformLocations.texture, 0);




	var angle = 0.0;
	var rotationspeed = 15.2;
	var camerapos = [0.0, 0.0, 0.0];

	requestAnimationFrame(drawScene);
	toggle();

	var then = 0;
	function drawScene(now){
		if(running){
			gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
			gl.clearColor(0.0, 0.0, 0.0, 1.0);
			
			// convert requestanimationframe timestamp to seconds
			now *= 0.001;
			// subtract the previous time from the current time
			var deltaTime = now - then;
			// remember the current time for the next frame
			then = now;

			angle += rotationspeed * deltaTime;

			// --- SETUP PROJECTION MATRIX --- (MAKE EVERYTHING 3D)
			//var projectionmatrix = m4.createOrthographicMatrix(0, gl.canvas.clientWidth, gl.canvas.clientHeight, 0, 400, -400);
			var projectionmatrix = m4.createPerspectiveMatrix(m4.degreeToRadians(46.0), gl.canvas.clientWidth/gl.canvas.clientHeight, 1, 200000);
			gl.uniformMatrix4fv(uniformLocations.projectionmatrix, false, projectionmatrix);


			// --- SETUP LOOKAT MATRIX ---
			var lookatmatrix = m4.createIdentityMatrix();
			lookatmatrix = m4.mult(m4.createTranslationMatrix(camerapos[0]+Math.sin(m4.degreeToRadians(viewxz)), camerapos[1]+Math.sin(m4.degreeToRadians(viewy)), camerapos[2]+Math.cos(m4.degreeToRadians(viewxz))), lookatmatrix);
			var lookatposition = [lookatmatrix[12], lookatmatrix[13], lookatmatrix[14]];


			// --- FIRST PERSON CAMERA --- 
			var factorws = (keys["w"] ? 1 : keys["s"] ? -1 : 0);
			lookatposition[0] += Math.sin(m4.degreeToRadians(viewxz))*10*factorws;
			lookatposition[1] += Math.sin(m4.degreeToRadians(viewy))*10*factorws;
			lookatposition[2] += Math.cos(m4.degreeToRadians(viewxz))*10*factorws;
			camerapos[0] += Math.sin(m4.degreeToRadians(viewxz))*10*factorws;
			camerapos[1] += Math.sin(m4.degreeToRadians(viewy))*10*factorws;
			camerapos[2] += Math.cos(m4.degreeToRadians(viewxz))*10*factorws;

			var factorad = (keys["d"] ? 1 : keys["a"] ? -1 : 0);
			var movcamvector = m4.cross([Math.sin(m4.degreeToRadians(viewxz)), Math.sin(m4.degreeToRadians(viewy)), Math.cos(m4.degreeToRadians(viewxz))], [0,1,0]);
			lookatposition[0] += movcamvector[0]*10*factorad;
			lookatposition[2] += movcamvector[2]*10*factorad;
			camerapos[0] += movcamvector[0]*10*factorad;
			camerapos[2] += movcamvector[2]*10*factorad;

			var factoreq = (keys["e"] ? 10 : keys["q"] ? -10 : 0);
			lookatposition[1] += factoreq;
			camerapos[1] += factoreq;
			

			// --- SETUP VIEWMATRIX --- (MOVE THE WORLD INVERSE OF THE CAMERAMOVEMENT)
			var cameramatrix = m4.lookAt(camerapos, lookatposition, [0, 1, 0]);
			var viewmatrix = m4.inverse(cameramatrix);
			var viewmatrixlocation = gl.getUniformLocation(program, "viewmatrix");
			gl.uniformMatrix4fv(uniformLocations.viewmatrix, false, viewmatrix);

			// -- DRAW ---
			for(var i = 0; i < 1; i++){

				r3webgl.connectBufferToAttribute(gl.ARRAY_BUFFER, vertexbuffer, attribLocations.vertexposition, 3, true);
				r3webgl.connectBufferToAttribute(gl.ARRAY_BUFFER, normalbuffer, attribLocations.normal, 3, true);
				r3webgl.connectBufferToAttribute(gl.ARRAY_BUFFER, texcoordbuffer, attribLocations.texturecoordinate, 2, true);
			
				
				gl.bindTexture(gl.TEXTURE_2D, texture);

				var modelmatrix = r3webgl.createModelMatrix(500, 0, 0, 0, -angle, 0, 1, 1, 1);
				gl.uniformMatrix4fv(uniformLocations.modelmatrix, false, modelmatrix);
				gl.uniformMatrix4fv(uniformLocations.inversetransposemodelmatrix, false, m4.transpose(m4.inverse(modelmatrix)));
				gl.drawArrays(gl.TRIANGLES, 0, 96);


				r3webgl.connectBufferToAttribute(gl.ARRAY_BUFFER, cubevertexbuffer, attribLocations.vertexposition, 3, true);
				r3webgl.connectBufferToAttribute(gl.ARRAY_BUFFER, cubenormalbuffer, attribLocations.normal, 3, true);
				r3webgl.connectBufferToAttribute(gl.ARRAY_BUFFER, cubetexcoordbuffer, attribLocations.texturecoordinate, 2, true);
			
				gl.bindTexture(gl.TEXTURE_2D, texturecube);

				var cubemodelmatrix = r3webgl.createModelMatrix(0, 0, 500, 0, /*angle*/0, 0, 100, 100, 100);
				gl.uniformMatrix4fv(uniformLocations.modelmatrix, false, cubemodelmatrix);
				gl.uniformMatrix4fv(uniformLocations.inversetransposemodelmatrix, false, m4.transpose(m4.inverse(modelmatrix)));
				gl.drawArrays(gl.TRIANGLES, 0, /*6*2*3*/ 20000000);
				

			}

		}
		requestAnimationFrame(drawScene);
	}
}
