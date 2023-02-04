#include "environment.hpp"
#include "basic_shape.hpp"
#include "vertex_attribute.hpp"
#include "Shader.hpp"
#include "build_shapes.hpp"


//Processes user input (keyboard currently).
//Receives a GLFWwindow pointer as input.
void processInput(GLFWwindow *window);

//Global Variables
glm::vec4 clear_color(0.0,0.0,0.0,1.0);
bool clear_key_pressed[] {false,false,false};

glm::vec4 offset_vec(0.0,0.0,0.0,0.0);

int main () {
    GLFWwindow *window = InitializeEnvironment("LearnOpenGL",600,600);
    if (window == NULL) {
        return -1;
    }
    
    Shader shader("./vertexShader.glsl","./fragmentShader.glsl");
    Shader textureShader ("./textureVertexShader.glsl","./textureFragmentShader.glsl");

    unsigned int texture = GetTexture("./images/space.jpg");
    unsigned int planetTexture = GetTexture("./images/mars.jpg");


    //1. define your Vertex Array Object, used to hold attribute pointers
    VAOStruct position_vao;
    glGenVertexArrays(1,&(position_vao.id));
    AttributePointer position_attr = BuildAttribute(3,GL_FLOAT,false,3*sizeof(float),0);
    position_vao.attributes.push_back(position_attr);

    VAOStruct texture_vao;
    glGenVertexArrays(1,&(texture_vao.id));
    AttributePointer pos_attr = BuildAttribute(3,GL_FLOAT,false,5*sizeof(float),0);
    AttributePointer text_attr = BuildAttribute(2,GL_FLOAT,false,5*sizeof(float),3*sizeof(float));
    texture_vao.attributes.push_back(pos_attr);
    texture_vao.attributes.push_back(text_attr);



    //These are the shapes for the rocket ship, we are not 
    //  drawing them here, we are just instantiated them. They will be drawn in the
    //  render loop
    BasicShape triangle = GetTriangle(position_vao,0.25,glm::vec3(0.0,0.15,0.0));
    BasicShape planet = GetTextureCircle(texture_vao,0.3,40,glm::vec3(0.7,0.6,0.0));
    BasicShape left_cannon = GetRectangle(position_vao,glm::vec3(0.18,-0.1,0.0),0.05,0.2);
    BasicShape right_cannon = GetRectangle(position_vao,glm::vec3(-0.23,-0.1,0.0),0.05,0.2);
    BasicShape hull = GetHull(position_vao,glm::vec3(-0.13,-0.1,0.0),0.26,0.4);
    BasicShape windshield = GetHull(position_vao,glm::vec3(-0.1,0.2,0.0),0.2,0.06);
    BasicShape stars = GetStars(position_vao,200);


    //Friendly tank, centered at the origin and facing in the x direction
    BasicShape left_track = GetRectangle(position_vao,glm::vec3(-0.4,0.15,0.0),0.8,0.15);
    BasicShape right_track = GetRectangle(position_vao,glm::vec3(-0.4,-0.3,0.0),0.8,0.15);
    BasicShape cabin = GetRectangle(position_vao,glm::vec3(-0.3,-0.15,0.0),0.6,0.3);
    BasicShape cannon = GetRectangle(position_vao,glm::vec3(0.0,-0.05,0.0),0.55,0.1);
    BasicShape turret = GetRectangle(position_vao,glm::vec3(-0.15,-0.1,0.0),0.3,0.2);


    //Enemy Tank
    BasicShape left_track_en = GetRectangle(position_vao,glm::vec3(-0.4,0.15,0.0),0.8,0.15);
    BasicShape right_track_en = GetRectangle(position_vao,glm::vec3(-0.4,-0.3,0.0),0.8,0.15);
    BasicShape cabin_en = GetRectangle(position_vao,glm::vec3(-0.3,-0.15,0.0),0.6,0.3);
    BasicShape cannon_en = GetRectangle(position_vao,glm::vec3(0.0,-0.05,0.0),0.55,0.1);
    BasicShape turret_en = GetTriangle(position_vao,0.25,glm::vec3(0.0,0.15,0.0));

    //Instantiating the spaceRect for texture that will be drawn later
    BasicShape spaceRect = GetTextureRectangle(texture_vao,glm::vec3(-1.0,-1.0,0.0),2.0,2.0);

    //vector for friendly tank parts
    std::vector<BasicShape*> friendly_parts {&left_track,&right_track,&cabin,
                                     &cannon,&turret};


    shader.use();
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

        //You'll DRAW the space texture here
        glBindTexture(GL_TEXTURE_2D,texture);
        //spaceRect.Draw(textureShader);

        //Draw the stars in the night sky
        // shader.use();
        // shader.setVec4("offset_vec",glm::vec4(0.0));
        // shader.setVec4("set_color",glm::vec4(1.0));
        // stars.Draw();
        // shader.setVec4("set_color",glm::vec4(0.7,0.0,0.0,1.0));
        
        //Draw the planet and its texture
        // glBindTexture(GL_TEXTURE_2D,planetTexture);
        // textureShader.use();
        // planet.Draw();
        // textureShader.setBool("use_set_color",true);
        // textureShader.setVec4("set_color",glm::vec4(0.0,0.0,0.0,1.0));
        // planet.DrawEBO();
        // textureShader.setBool("use_set_color",false);
        // shader.use();


        //Draw ship (later, you should handle this with a 'Friendly Tank' class)
        //  loop through the "shapes" vector we made earlier
        shader.setVec4("offset_vec",offset_vec);
        for (int i = 0; i < friendly_parts.size(); i++) {
            shader.setVec4("set_color",glm::vec4(0.0,0.502,0,1.0));
            friendly_parts[i]->Draw();
            shader.setVec4("set_color",glm::vec4(1.0));
            friendly_parts[i]->DrawEBO();
        }

        //      DRAW FRIENDLY TANK        //
        // //Left Track
        // shader.setVec4("set_color", glm::vec4(0,1.0,0,1.0));
        // left_track.Draw();
        // shader.setVec4("set_color", glm::vec4(1.0));
        // left_track.DrawEBO(2.0);

        // //Right Track
        // shader.setVec4("set_color", glm::vec4(0,1.0,0,1.0));
        // right_track.Draw();
        // shader.setVec4("set_color", glm::vec4(1.0));
        // right_track.DrawEBO(2.0);

        // //Cabin
        // shader.setVec4("set_color", glm::vec4(0,1.0,0,1.0));
        // cabin.Draw();
        // shader.setVec4("set_color", glm::vec4(1.0));
        // cabin.DrawEBO(2.0);

        // //Cannon
        // shader.setVec4("set_color", glm::vec4(0,1.0,0,1.0));
        // cannon.Draw();
        // shader.setVec4("set_color", glm::vec4(1.0));
        // cannon.DrawEBO(2.0);

        // //Turret
        // shader.setVec4("set_color", glm::vec4(0,1.0,0,1.0));
        // turret.Draw();
        // shader.setVec4("set_color", glm::vec4(1.0));
        // turret.DrawEBO(2.0);
        //      END OF FRIENDLY TANK        //



        

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
    if(glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_RELEASE){
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
    //Arrow Key processing
    int dir[] {GLFW_KEY_UP,GLFW_KEY_DOWN,GLFW_KEY_RIGHT,GLFW_KEY_LEFT};
    int vals[] {1,-1,1,-1};
    int offset_vals[] {1,1,0,0};
    float move_amount = 0.001;
    for (int i = 0; i < 4; i++) {
        if (glfwGetKey(window,dir[i])==GLFW_PRESS) {
            offset_vec[offset_vals[i]]+=vals[i]*move_amount;
        }
    }
}



