#ifndef TANK_HPP

#define TANK_HPP

#include "build_shapes.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//An example composite shape class with some basic functionality.
//  Note that a weakness of this class is that if you inherit from it 
//  you need to overwrite the Draw function and some of the data members don't 
//  make a lot of sense.  HOWEVER, this is easily changed if you use maps or 
//  vectors of shapes, shaders, textures, etc... to draw the objects (then all you
//  need to do is override the Create data member function).
class Tank {
    protected:
        VAOStruct basicVAO;
        VAOStruct textureVAO;
        Shader *basicShader;
        Shader *textureShader;

        //Tank shapes
        BasicShape left_track;
        BasicShape right_track;
        BasicShape cabin;
        BasicShape cannon;
        BasicShape turret;

 


        float angleZ = 0.0;
        glm::vec4 location = glm::vec4(0.0,0.0,0.0,1.0);
        unsigned int hullTexture = 0;
    
    public:
        //Constructor for the Tank class
        Tank();

        //Set up the VAOs and shaders for the Tank.
        void CreateTank (VAOStruct basic, VAOStruct texture, Shader *bShader, Shader *tShader);
        
        //Draw the Tank
        void Draw ();

        //Set the rotation of the ship
        void SetRotation(float angle);

        //Set the location of the ship
        void SetLocation(glm::vec4 newLocation);

        //Delete the VBOs
        ~Tank();
};


#endif //Tank_HPP