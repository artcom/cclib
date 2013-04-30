//uniform sampler2D texture;
uniform sampler2DRect texture;

uniform float highlightRange;
uniform float highlightScale;
uniform float highlightPow;
		
vec4 saturate(vec4 inp) { 
  return clamp(inp, 0.0, 1.0); 
} 

void main(void) { 

  // Texturen auslesen 
  //vec4 color = texture2D(texture, gl_TexCoord[0].xy ); 
  vec4 color = texture2DRect(texture, gl_FragCoord.xy ); 
  //gl_FragColor = color; return;
  
  //Kontrast 
  /*
  vec4 c = vec4(2.0,2.0,2.0,1.0) * (vec4(1.0,1.0,1.0,1.0) - contrast); 
  outp = saturate((outp - vec4(0.5,0.5,0.5,0.5)) * c + vec4(0.5,0.5,0.5,0.5)); 

  //Farben pushen 
  outp *= brightness; 

  //Helligkeit 
  vec4 b = (vec4(1.0,1.0,1.0,1.0) - contrast * vec4(2.0,2.0,2.0,1.0)); 
  outp = saturate(outp + b); 
*/
	float brightness = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
	brightness -= highlightRange;
	brightness /= 1.0 - highlightRange;
	brightness = pow(brightness,highlightPow) * highlightScale;
  gl_FragColor =  color * brightness; 
} 