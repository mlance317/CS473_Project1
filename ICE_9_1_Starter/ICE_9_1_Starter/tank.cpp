#include "tank.hpp"

Tank::Tank () {
 
}

void Tank::CreateTank (VAOStruct basic, VAOStruct texture, Shader *bShader, Shader *tShader) {

    //VAOs and Shaders
    this->basicVAO = basic;
    this->basicShader = bShader;
    this->textureVAO = texture;
    this->textureShader = tShader;


    //Tank Shapes
    this->left_track = GetRectangle(this->basicVAO,glm::vec3(-0.4,0.15,0.0),0.8,0.15);
    this->right_track = GetRectangle(this->basicVAO,glm::vec3(-0.4,-0.3,0.0),0.8,0.15);
    this->cabin = GetRectangle(this->basicVAO,glm::vec3(-0.3,-0.15,0.0),0.6,0.3);
    this->cannon = GetRectangle(this->basicVAO,glm::vec3(0.0,-0.05,0.0),0.55,0.1);
    this->turret = GetRectangle(this->basicVAO,glm::vec3(-0.15,-0.1,0.0),0.3,0.2);

    //Textures
    this->hullTexture = GetTexture("./images/hull_texture.png");
}

void Tank::SetRotation(float angle) {
    this->angleZ = angle;
}

void Tank::SetLocation(glm::vec4 newLocation) {
    this->location = newLocation;
}

void Tank::Draw () {
    this->basicShader->use();
    glm::mat4 trans (1.0f);

    //Transformations
    trans = glm::translate(trans,glm::vec3(this->location));
    trans = glm::translate(trans,glm::vec3(0.0,0.0,0.0));
    trans = glm::scale(trans,glm::vec3(0.2,0.2,1.0));
    trans = glm::rotate(trans,glm::radians(this->angleZ),glm::vec3(0.0,0.0,1.0));
    trans = glm::rotate(trans,glm::radians(0.0f),glm::vec3(0.0,0.0,1.0));
    trans = glm::translate(trans,glm::vec3(0.0,0.0,0.0));


    //Tank Shapes

    //left right tracks
    this->basicShader->setMat4("transform",trans);
    this->basicShader->setVec4("set_color",glm::vec4(0.0,0.502,0,1.0));
    this->left_track.Draw();
    this->right_track.Draw();
    this->basicShader->setVec4("set_color",glm::vec4(1.0));
    this->left_track.DrawEBO();
    this->right_track.DrawEBO();

    //cabin
    this->basicShader->setVec4("set_color",glm::vec4(0.0,0.502,0,1.0));
    this->cabin.Draw();
    this->basicShader->setVec4("set_color",glm::vec4(1.0));
    this->cabin.DrawEBO();

    //cannon
    this->basicShader->setVec4("set_color",glm::vec4(0.0,0.502,0,1.0));
    this->cannon.Draw();
    this->basicShader->setVec4("set_color",glm::vec4(1.0));
    this->cannon.DrawEBO();

    //turret
    this->basicShader->setVec4("set_color",glm::vec4(0.0,0.502,0,1.0));
    this->turret.Draw();
    this->basicShader->setVec4("set_color",glm::vec4(1.0));
    this->turret.DrawEBO();

    // this->textureShader->use();
    // glBindTexture(GL_TEXTURE_2D,this->hullTexture);
    // this->textureShader->setMat4("transform",trans);
    // this->hull.Draw();
    // this->textureShader->setBool("use_color",true);
    // this->textureShader->setVec4("set_color",glm::vec4(0.0,0.0,0.0,1.0));
    // this->hull.DrawEBO();
    // this->textureShader->setBool("use_color",false);
    // this->basicShader->use();
    // this->basicShader->setVec4("set_color",glm::vec4(0.4,0.4,0.9,1.0));
    // this->windshield.Draw();
    // this->basicShader->setVec4("set_color",glm::vec4(0.0,0.0,0.0,1.0));
    // this->windshield.DrawEBO();
    
    
}

Tank::~Tank() {
    //Delete the buffers when the tank goes out of scope.
        unsigned int lt = this->left_track.GetVBO();
        glDeleteBuffers(1,&lt);
        unsigned int rt = this->right_track.GetVBO();
        glDeleteBuffers(1,&rt);
        unsigned int cab = this->cabin.GetVBO();
        glDeleteBuffers(1,&cab);
        unsigned int can = this->cannon.GetVBO();
        glDeleteBuffers(1,&can);
        unsigned int tur = this->turret.GetVBO();
        glDeleteBuffers(1,&tur);

}