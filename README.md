cclib
=====

c++ port of the creative computing particle system (http://code.google.com/p/creativecomputing)


Classes that we need to port:

Math:
cc.creativecomputing.math.CCVector2;                
cc.creativecomputing.math.CCVector3;
cc.creativecomputing.math.CCPlane3;
cc.creativecomputing.math.CCMath;                   - not the whole class is needed. port on demand -> Only used for random vectors 
cc.creativecomputing.math.CCVecMath;                - same as CCVecMath. On Demand. -> Only used for random vectors

GL:
cc.creativecomputing.graphics.CCDrawMode;
cc.creativecomputing.graphics.CCGraphics;
cc.creativecomputing.graphics.shader.CCCGShader;
cc.creativecomputing.graphics.shader.CCShaderTexture;

CG:
com.jogamp.opengl.cg.CGparameter;
com.jogamp.opengl.cg.CgGL;

Particles:
cc.creativecomputing.simulation.gpuparticles.CCGPUQueueParticles;
cc.creativecomputing.simulation.gpuparticles.CCGPUParticles;
cc.creativecomputing.simulation.gpuparticles.CCGPUVelocityShader;

cc.creativecomputing.simulation.gpuparticles.constraints.CCGPUConstraint;
cc.creativecomputing.simulation.gpuparticles.constraints.CCGPUYConstraint;

cc.creativecomputing.simulation.gpuparticles.forces.CCGPUForce;
cc.creativecomputing.simulation.gpuparticles.forces.CCGPUForceField;
cc.creativecomputing.simulation.gpuparticles.forces.CCGPUGravity;
cc.creativecomputing.simulation.gpuparticles.forces.CCGPUViscousDrag;

cc.creativecomputing.simulation.gpuparticles.impulses.CCGPUImpulse;

cc.creativecomputing.simulation.gpuparticles.render.CCGPUParticlePointRenderer;
cc.creativecomputing.simulation.gpuparticles.render.CCGPUParticleRenderer;



