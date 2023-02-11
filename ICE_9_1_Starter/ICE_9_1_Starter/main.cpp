#include "environment.hpp"
#include "basic_shape.hpp"
#include "vertex_attribute.hpp"
#include "Shader.hpp"
#include "build_shapes.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "spaceship.hpp"
#include "tank.hpp"
#include "bullet.hpp"


//Processes user input (keyboard currently).
//Receives a GLFWwindow pointer as input.
void processInput(GLFWwindow *window);

//Global Variables
glm::vec4 clear_color(0.0,0.0,0.0,1.0);
bool clear_key_pressed[] {false,false,false};

glm::vec4 offset_vec(0.0,0.0,0.0,0.0);
glm::vec4 offset_vec_bullet(0.0,0.0,0.0,0.0);
float angle_z = 0.0f;
float angle_z_bullet = 0.0f;
glm::vec4 velocity (0.0,0.0,0.0,0.0);
glm::vec4 bulletVelo (0.0,0.0,0.0,0.0);
bool moveTracks = false;
float speed = 0.000075;
bool fireCannon = false;


int main () {
    GLFWwindow *window = InitializeEnvironment("LearnOpenGL",600,600);
    if (window == NULL) {
        return -1;
    }

    unsigned int space_texture = GetTexture("./images/space.jpg");
    unsigned int planet_texture = GetTexture("./images/mars.jpg");

    
    Shader shader("./vertexShader.glsl","./fragmentShader.glsl");
    Shader texture_shader("./textureVertexShader.glsl","./textureFragmentShader.glsl");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    //1. define your Vertex Array Object, used to hold attribute pointers
    VAOStruct position_vao;
    glGenVertexArrays(1,&(position_vao.id));
    AttributePointer position_attr = BuildAttribute(3,GL_FLOAT,false,3*sizeof(float),0);
    position_vao.attributes.push_back(position_attr);


    VAOStruct texture_vao;
    glGenVertexArrays(1,&(texture_vao.id));
    AttributePointer texture_pos_attr = BuildAttribute(3,GL_FLOAT,false,5*sizeof(float),0);
    AttributePointer texture_attr = BuildAttribute(2,GL_FLOAT,false,5*sizeof(float),3*sizeof(float));
    texture_vao.attributes.push_back(texture_pos_attr);
    texture_vao.attributes.push_back(texture_attr);

    //Stuff for spaceships
    Spaceship ship;
    ship.CreateSpaceship(position_vao,texture_vao,&shader,&texture_shader);

    //Stuff for tanks
    Tank friendly_tank;
    friendly_tank.CreateTank(position_vao,texture_vao,&shader,&texture_shader);

    //stuff for bullets
    Bullet sabo;
    sabo.CreateBullet(position_vao,texture_vao,&shader,&texture_shader);


    //Background stuff and planets
    BasicShape planet = GetTexturedCircle(texture_vao,0.3,40,glm::vec3(0.7,0.6,0.0));
    BasicShape stars = GetStars(position_vao,200);
    BasicShape space = GetTexturedRectangle(texture_vao,glm::vec3(-1.0,-1.0,0.0),2.0,2.0);

    
    //The render loop -- the function in the condition checks if the 
    //  window has been set to close (does this each iteration)
    while (!glfwWindowShouldClose(window)) {
        //input
        processInput(window);

        //render commands here
        //set the clear color to wipe the window
        glClearColor(clear_color.r,clear_color.g,clear_color.b,clear_color.a);


        //clear the color buffer (where things are drawn) using the current clear color.
        glClear(GL_COLOR_BUFFER_BIT);
        glPointSize(3.0);
        shader.setMat4("transform",glm::mat4(1.0f));
        glBindTexture(GL_TEXTURE_2D,space_texture);
        texture_shader.use();
        texture_shader.setMat4("transform",glm::mat4(1.0f));
        space.Draw();
        shader.use();
        shader.setVec4("set_color",glm::vec4(1.0));
        stars.Draw();
        texture_shader.use();
        glBindTexture(GL_TEXTURE_2D,planet_texture);
        planet.Draw();
        texture_shader.setBool ("use_color",true);
        texture_shader.setVec4("set_color",glm::vec4(0.0,0.0,0.0,1.0));
        planet.DrawEBO();
        texture_shader.setBool("use_color",false);

        //Draw ship...notice how much this cleans up your render loop!
        // ship.SetRotation(angle_z);
        // ship.SetLocation(offset_vec);
        // ship.Draw();


        //Draw tankw
        friendly_tank.SetRotation(angle_z);
        friendly_tank.SetLocation(offset_vec);
        friendly_tank.Draw();

        //Draw Friendly Bullet
        sabo.SetRotation(angle_z_bullet);
        sabo.SetLocation(offset_vec_bullet);
        sabo.Draw();


        offset_vec += velocity;
        offset_vec_bullet += bulletVelo;

        
        //check and call events and swap the buffers

        //Applications are usually drawn in a double buffer...one for drawing one for displaying
        //you draw to one and then you swap the buffers...avoids flickering.
        glfwSwapBuffers(window);

        //checks if any events are triggered (keyboard or mouse input)
        glfwPollEvents();
    }

    //terminate your window when you leave the render loop
    glfwTerminate();
    //must have a return value 0 indicates successful run...in this case yes.
    return 0;

}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Change the W to a Y
    if(glfwGetKey(window,GLFW_KEY_Y)==GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
     //Change the W to a Y
    if(glfwGetKey(window,GLFW_KEY_Y) == GLFW_RELEASE){
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); 
    }
    //Color Key Processing
    int keys[] {GLFW_KEY_R,GLFW_KEY_G,GLFW_KEY_B};
    for (int i = 0; i < 3; i++) {
        if ((glfwGetKey(window,keys[i])==GLFW_PRESS) && (!clear_key_pressed[i])){
            clear_key_pressed[i] = !clear_key_pressed[i];
            clear_color[i] += 0.1;
            if (clear_color[i] > 1.1) {
                clear_color[i] = 0.0;
            }
            PrintColor(clear_color);
        }
        if (glfwGetKey(window,keys[i])==GLFW_RELEASE) {
            clear_key_pressed[i] = false;
        }
    }
    if (glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS) {
        angle_z += 0.05;
        if (fireCannon == false){
            angle_z_bullet += 0.05;
        }
    }
    if (glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS) {
        angle_z -= 0.05;
        if (fireCannon == false){
            angle_z_bullet -= 0.05;
        }
    }




    //Move the tank forward
    if (glfwGetKey(window,GLFW_KEY_S)==GLFW_RELEASE) {
        moveTracks = false;
        velocity.x = 0;
        velocity.y = 0;
        if (!fireCannon) {
            bulletVelo.x = 0;
            bulletVelo.y = 0;
        }
    }

    if (!(moveTracks) && (glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)) {
        moveTracks = true;
        velocity.x += speed*cos(glm::radians(angle_z));
        velocity.y += speed*sin(glm::radians(angle_z));
        if (fireCannon == false) {
            bulletVelo.x += speed*cos(glm::radians(angle_z_bullet));
            bulletVelo.y += speed*sin(glm::radians(angle_z_bullet));
        }
    }

    //Move tank backward
    if (glfwGetKey(window,GLFW_KEY_W)==GLFW_RELEASE) {
        moveTracks = false;
        velocity.x = 0;
        velocity.y = 0;
        if (!fireCannon) {
            bulletVelo.x = 0;
            bulletVelo.y = 0;
        }
    }

    if (!(moveTracks) && (glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS)) {
        moveTracks = true;
        velocity.x -= speed*cos(glm::radians(angle_z));
        velocity.y -= speed*sin(glm::radians(angle_z));
        if (fireCannon == false){
            bulletVelo.x -= speed*cos(glm::radians(angle_z_bullet));
            bulletVelo.y -= speed*sin(glm::radians(angle_z_bullet));
        }

    }
    //Fire a round    
    if (!(fireCannon) && (glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS)) {
        fireCannon = true;
        bulletVelo.x += ((speed*2) *cos(glm::radians(angle_z_bullet)));
        bulletVelo.y += ((speed*2) *sin(glm::radians(angle_z_bullet)));
    }

    //Reload round
    if ((fireCannon == true) && (glfwGetKey(window,GLFW_KEY_F)==GLFW_PRESS)) {
        fireCannon = false;
        bulletVelo.x = 0.0;
        bulletVelo.y = 0.0;
        offset_vec_bullet.x = offset_vec.x;
        offset_vec_bullet.y = offset_vec.y;
        angle_z_bullet = angle_z;

    }

    //Boundaries
    if ((offset_vec.x + velocity.x < -1.0) || (offset_vec.x + velocity.x > 1.0)) {
        velocity.x = 0.0f;
    }
    if ((offset_vec.y + velocity.y < -1.0) || (offset_vec.y + velocity.y > 1.0)) {
        velocity.y = 0.0f;
    }


}



