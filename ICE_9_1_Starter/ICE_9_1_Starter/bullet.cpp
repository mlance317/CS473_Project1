#include "bullet.hpp"

Bullet::Bullet () {
 
}

void Bullet::CreateBullet (VAOStruct basic, VAOStruct texture, Shader *bShader, Shader *tShader) {

    //VAOs and Shaders
    this->basicVAO = basic;
    this->basicShader = bShader;
    this->textureVAO = texture;
    this->textureShader = tShader;


    //Bullet Shapes
    this->shell = GetRectangle(this->basicVAO,glm::vec3(0.0,-0.03,0.0),0.15,0.06);
    //this->tip = GetTriangle(this->basicVAO,0.2f, glm::vec3(0.0,0.0,0.0));


    //Textures
    this->hullTexture = GetTexture("./images/hull_texture.png");
}

void Bullet::SetRotation(float angle) {
    this->angleZ = angle;
}

void Bullet::SetLocation(glm::vec4 newLocation) {
    this->location = newLocation;
}

void Bullet::Draw () {
    this->basicShader->use();
    glm::mat4 trans (1.0f);

    //Transformations
    trans = glm::translate(trans,glm::vec3(this->location));
    trans = glm::translate(trans,glm::vec3(0.0,0.0,0.0));
    trans = glm::scale(trans,glm::vec3(0.2,0.2,1.0));
    trans = glm::rotate(trans,glm::radians(this->angleZ),glm::vec3(0.0,0.0,1.0));
    trans = glm::rotate(trans,glm::radians(0.0f),glm::vec3(0.0,0.0,1.0));
    trans = glm::translate(trans,glm::vec3(0.0,0.0,0.0));


    //Bullet Shapes

    this->basicShader->setMat4("transform",trans);
    this->basicShader->setVec4("set_color",glm::vec4(0.7216,0.749,0.7608,1.0));
    this->shell.Draw();
    //this->tip.Draw();
    this->basicShader->setVec4("set_color",glm::vec4(1.0));
    this->shell.DrawEBO();
    //this->tip.DrawEBO();
    
    
}

Bullet::~Bullet() {
    //Delete the buffers when the bullet goes out of scope.
        unsigned int sh = this->shell.GetVBO();
        glDeleteBuffers(1,&sh);
        //unsigned int ti = this->tip.GetVBO();
        //glDeleteBuffers(1,&ti);
 

}