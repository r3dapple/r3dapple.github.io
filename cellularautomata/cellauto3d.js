/*
* Copyright 2021 Markus Heimerl, OTH Regensburg, r3dapple.de
* Licensed under CC BY-NC 4.0
* r3dapple.de/LICENSE.txt
*
* ANY USE OF THIS SOFTWARE MUST COMPLY WITH THE
* CREATIVE COMMONS ATTRIBUTION-NONCOMMERCIAL 4.0 INTERNATIONAL LICENSE
* AGREEMENTS
*/

cellularautomata3d();
function cellularautomata3d(){

	function createCellGridWireframe(cellularworldsize, randomize){
		var cellgrid = [];
		for(var x = 0; x < cellularworldsize; x++){
			cellgrid[x] = [];
			for(var y = 0; y < cellularworldsize; y++){
				cellgrid[x][y] = [];
				for(var z = 0; z < cellularworldsize; z++){
					if(randomize)cellgrid[x][y][z] = Math.random() >= 0.5 ? 1 : 0;
					else cellgrid[x][y][z] = 0;
					
					// mark world edges
					if(y == 0 && z == 0) cellgrid[x][y][z] = 1;
					if(x == 0 && z == 0) cellgrid[x][y][z] = 1;
					if(x == 0 && y == 0) cellgrid[x][y][z] = 1;
					
					if(y == cellularworldsize-1 && z == cellularworldsize-1) cellgrid[x][y][z] = 1;
					if(x == cellularworldsize-1 && z == cellularworldsize-1) cellgrid[x][y][z] = 1;
					if(x == cellularworldsize-1 && y == cellularworldsize-1) cellgrid[x][y][z] = 1;
					
					if(y == cellularworldsize-1 && z == 0) cellgrid[x][y][z] = 1;
					if(x == cellularworldsize-1 && z == 0) cellgrid[x][y][z] = 1;
					if(x == cellularworldsize-1 && y == 0) cellgrid[x][y][z] = 1;
					
					if(y == 0 && z == cellularworldsize-1) cellgrid[x][y][z] = 1;
					if(x == 0 && z == cellularworldsize-1) cellgrid[x][y][z] = 1;
					if(x == 0 && y == cellularworldsize-1) cellgrid[x][y][z] = 1;
				}
			}
		}
		return cellgrid;
	}

	var cellularworldsize = 60;
	var cellgrid = createCellGridWireframe(cellularworldsize, true);
	var rulefactor = 2;

	cellularAutomataLogic();
	function cellularAutomataLogic(){
		if(running){
			
			var cellgridnextframe = createCellGridWireframe(cellularworldsize, false);
			
			for(var x = 2; x < cellularworldsize-2; x++){
				for(var y = 2; y < cellularworldsize-2; y++){
					for(var z = 2; z < cellularworldsize-2; z++){
						// get cell status
						var status = cellgrid[x][y][z];
						
						// get neighbor count
						var zm1 = z-1 < 2 ? cellularworldsize-3 : z-1;
						var zp1 = z+1 == cellularworldsize-2 ? 2 : z+1;
						var ym1 = y-1 < 2 ? cellularworldsize-3 : y-1;
						var yp1 = y+1 == cellularworldsize-2 ? 2 : y+1;
						var xm1 = x-1 < 2 ? cellularworldsize-3 : x-1;
						var xp1 = x+1 == cellularworldsize-2 ? 2 : x+1;
						
						// below
						var neighborcount = cellgrid[x][y][zm1] + cellgrid[x][yp1][zm1] + cellgrid[x][ym1][zm1] + cellgrid[xp1][y][zm1] 
											+ cellgrid[xm1][y][zm1] + cellgrid[xp1][yp1][zm1] + cellgrid[xp1][ym1][zm1] + cellgrid[xm1][ym1][zm1] + cellgrid[xm1][yp1][zm1];
						
						// level
						neighborcount += cellgrid[x][yp1][z] + cellgrid[x][ym1][z] + cellgrid[xp1][y][z] 
											+ cellgrid[xm1][y][z] + cellgrid[xp1][yp1][z] + cellgrid[xp1][ym1][z] + cellgrid[xm1][ym1][z] + cellgrid[xm1][yp1][z];
											
						// above
						neighborcount += cellgrid[x][y][zp1] + cellgrid[x][yp1][zp1] + cellgrid[x][ym1][zp1] + cellgrid[xp1][y][zp1] 
											+ cellgrid[xm1][y][zp1] + cellgrid[xp1][yp1][zp1] + cellgrid[xp1][ym1][zp1] + cellgrid[xm1][ym1][zp1] + cellgrid[xm1][yp1][zp1];
											
						// if cell alive and neiborcount 2,3 or 8, it stays alive
						if(status == 1 && (/*neighborcount == 2*rulefactor ||*/ neighborcount == 26 || neighborcount == 10*rulefactor)){
							cellgridnextframe[x][y][z] = 1;
						}
						// if cell is dead and neighborcount is 3, it is born
						else if(status == 0 && (/*neighborcount == 5 ||*/ neighborcount == 4)){
							cellgridnextframe[x][y][z] = 1;
						}
						// in all other cases the cell remains dead or dies (already initialized as 0)
					}
				}
			}
			
			cellgrid = cellgridnextframe;
		}
		setTimeout(cellularAutomataLogic, 100);
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
	const uniformLocations = r3webgl.getUniformLocations(program, ["modelmatrix", "viewmatrix", "projectionmatrix", "texture", "reverseLightDirection"]);

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
			var movementspeed = 0.8;
			var factorws = (keys["w"] ? 1 : keys["s"] ? -1 : 0);
			lookatposition[0] += Math.sin(m4.degreeToRadians(viewxz))*movementspeed*factorws;
			lookatposition[1] += Math.sin(m4.degreeToRadians(viewy))*movementspeed*factorws;
			lookatposition[2] += Math.cos(m4.degreeToRadians(viewxz))*movementspeed*factorws;
			camerapos[0] += Math.sin(m4.degreeToRadians(viewxz))*movementspeed*factorws;
			camerapos[1] += Math.sin(m4.degreeToRadians(viewy))*movementspeed*factorws;
			camerapos[2] += Math.cos(m4.degreeToRadians(viewxz))*movementspeed*factorws;

			var factorad = (keys["d"] ? 1 : keys["a"] ? -1 : 0);
			var movcamvector = m4.cross([Math.sin(m4.degreeToRadians(viewxz)), Math.sin(m4.degreeToRadians(viewy)), Math.cos(m4.degreeToRadians(viewxz))], [0,1,0]);
			lookatposition[0] += movcamvector[0]*movementspeed*factorad;
			lookatposition[2] += movcamvector[2]*movementspeed*factorad;
			camerapos[0] += movcamvector[0]*movementspeed*factorad;
			camerapos[2] += movcamvector[2]*movementspeed*factorad;

			var factoreq = (keys["e"] ? movementspeed : keys["q"] ? -movementspeed : 0);
			lookatposition[1] += factoreq;
			camerapos[1] += factoreq;
			

			// --- SETUP VIEWMATRIX --- (MOVE THE WORLD INVERSE OF THE CAMERAMOVEMENT)
			var cameramatrix = m4.lookAt(camerapos, lookatposition, [0, 1, 0]);
			var viewmatrix = m4.inverse(cameramatrix);
			var viewmatrixlocation = gl.getUniformLocation(program, "viewmatrix");
			gl.uniformMatrix4fv(uniformLocations.viewmatrix, false, viewmatrix);


			// --- CONNECT BUFFERS TO ATTRIBUTES --- (only has to be done once since the only object vertex data we ever need is that of a cube)
			r3webgl.connectBufferToAttribute(gl.ARRAY_BUFFER, cubevertexbuffer, attribLocations.vertexposition, 3, true);
			r3webgl.connectBufferToAttribute(gl.ARRAY_BUFFER, cubenormalbuffer, attribLocations.normal, 3, true);
			r3webgl.connectBufferToAttribute(gl.ARRAY_BUFFER, cubetexcoordbuffer, attribLocations.texturecoordinate, 2, true);
			
			
			// -- DRAW ---
			gl.bindTexture(gl.TEXTURE_2D, texturecube);
			for(var x = 0; x < cellularworldsize; x++){
				for(var y = 0; y < cellularworldsize; y++){
					for(var z = 0; z < cellularworldsize; z++){
						if(cellgrid[x][y][z] == 1){
							gl.uniformMatrix4fv(uniformLocations.modelmatrix, false, r3webgl.createModelMatrix(x, y, z, 0, 0, 0, 0.5, 0.5, 0.5));
							gl.drawArrays(gl.TRIANGLES, 0, 6*2*3 /*this has a lot less*/);
						}
					}
				}
			}

		}
		requestAnimationFrame(drawScene);
	}
}
