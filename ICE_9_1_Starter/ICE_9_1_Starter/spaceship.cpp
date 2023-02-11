#include "spaceship.hpp"

Spaceship::Spaceship () {
 
}

void Spaceship::CreateSpaceship (VAOStruct basic, VAOStruct texture, Shader *bShader, Shader *tShader) {

    //VAOs and Shaders
    this->basicVAO = basic;
    this->basicShader = bShader;
    this->textureVAO = texture;
    this->textureShader = tShader;

    //Shapes
    this->leftCannon = GetRectangle(this->basicVAO,glm::vec3(0.18,-0.1,0.0),0.05,0.2);
    this->rightCannon = GetRectangle(this->basicVAO,glm::vec3(-0.23,-0.1,0.0),0.05,0.2);
    this->hull = GetHull(this->textureVAO,glm::vec3(-0.13,-0.1,0.0),0.26,0.4,true);
    this->windshield = GetHull(this->basicVAO,glm::vec3(-0.1,0.2,0.0),0.2,0.06);
    this->wing = GetTriangle(this->basicVAO,0.25,glm::vec3(0.0,0.15,0.0));

    //Textures
    this->hullTexture = GetTexture("./images/hull_texture.png");
}

void Spaceship::SetRotation(float angle) {
    this->angleZ = angle;
}

void Spaceship::SetLocation(glm::vec4 newLocation) {
    this->location = newLocation;
}

void Spaceship::Draw () {
    this->basicShader->use();
    glm::mat4 trans (1.0f);
    trans = glm::translate(trans,glm::vec3(this->location));
    trans = glm::translate(trans,glm::vec3(0.0,-0.1,0.0));
    trans = glm::scale(trans,glm::vec3(0.2,0.2,1.0));
    trans = glm::rotate(trans,glm::radians(this->angleZ),glm::vec3(0.0,0.0,1.0));
    trans = glm::rotate(trans,glm::radians(-90.0f),glm::vec3(0.0,0.0,1.0));
    trans = glm::translate(trans,glm::vec3(0.0,-0.1,0.0));
    this->basicShader->setMat4("transform",trans);
    this->basicShader->setVec4("set_color",glm::vec4(0.7,0.3,0.3,1.0));
    this->leftCannon.Draw();
    this->rightCannon.Draw();
    this->basicShader->setVec4("set_color",glm::vec4(0.0));
    this->leftCannon.DrawEBO();
    this->rightCannon.DrawEBO();
    this->basicShader->setVec4("set_color",glm::vec4(0.5,0.5,0.6,1.0));
    this->wing.Draw();
    this->basicShader->setVec4("set_color",glm::vec4(0.0));
    this->wing.DrawEBO();
    this->textureShader->use();
    glBindTexture(GL_TEXTURE_2D,this->hullTexture);
    this->textureShader->setMat4("transform",trans);
    this->hull.Draw();
    this->textureShader->setBool("use_color",true);
    this->textureShader->setVec4("set_color",glm::vec4(0.0,0.0,0.0,1.0));
    this->hull.DrawEBO();
    this->textureShader->setBool("use_color",false);
    this->basicShader->use();
    this->basicShader->setVec4("set_color",glm::vec4(0.4,0.4,0.9,1.0));
    this->windshield.Draw();
    this->basicShader->setVec4("set_color",glm::vec4(0.0,0.0,0.0,1.0));
    this->windshield.DrawEBO();
    
    
}

Spaceship::~Spaceship() {
    //Delete the buffers when the ship goes out of scope.
        unsigned int lCan = this->leftCannon.GetVBO();
        glDeleteBuffers(1,&lCan);
        unsigned int rCan = this->rightCannon.GetVBO();
        glDeleteBuffers(1,&rCan);
        unsigned int wID = this->wing.GetVBO();
        glDeleteBuffers(1,&wID);
        unsigned int hID = this->hull.GetVBO();
        glDeleteBuffers(1,&hID);
        unsigned int shieldID = this->windshield.GetVBO();
        glDeleteBuffers(1,&shieldID);



}