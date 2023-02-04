#include "build_shapes.hpp"
#include "stb_image_implementation.hpp"

unsigned int GetTexture (std::string fileName, int wrapS, int wrapT)
{
   unsigned int texture;
   glGenTextures(1,&texture);
   glBindTexture(GL_TEXTURE_2D,texture);

   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrapS);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrapT);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

   //load and generate the texture
   int width, height, nrChannels;
   unsigned char *data = stbi_load(fileName.c_str(),&width,&height,&nrChannels,0);
    int channelType = GL_RGB;
    if (nrChannels == 4) {
        channelType = GL_RGBA;
    }

   if (data) {
    glTexImage2D(GL_TEXTURE_2D,0,channelType,width,height,0,channelType,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
   } else {
    std::cout << "Failed to load texture:" << fileName.c_str() << std::endl;
   }
   stbi_image_free(data);
   return texture;

}

BasicShape GetTriangle (VAOStruct vao,float size, glm::vec3 location) {
    float tri_vert[] {
        -1.0,-1.0,0.0,//bottom left vertex
        1.0,-1.0,0.0,  //bottom right vertex
        0.0,1.0,0.0  //top vertex
    };
    for (int i = 0; i < 3; i++) {
        tri_vert[3*i] = size * tri_vert[3*i] + location.x;
        tri_vert[3*i+1] = size * tri_vert[3*i+1] + location.y;
        tri_vert[3*i+2] = size * tri_vert[3*i+2] + location.z;
    }
    BasicShape new_shape;
    new_shape.Initialize(vao,tri_vert,sizeof(tri_vert),3,GL_TRIANGLES);

    unsigned int indices[] = {0,1,2};
    //Set the EBO for the shape here.
    new_shape.InitializeEBO(indices,sizeof(indices),3);
    return new_shape;
}

BasicShape GetRectangle (VAOStruct vao, glm::vec3 location, float width, float height)
{
    float vertices[] 
    {
        location.x, location.y, 0.0,
        location.x+width, location.y, 0.0,
        location.x+width, location.y+height,0.0,
        location.x, location.y+height, 0.0
    };
    BasicShape new_shape;
    new_shape.Initialize(vao,vertices,sizeof(vertices),4,GL_TRIANGLE_FAN);
    unsigned int indices[] = {0,1,2,3};
    new_shape.InitializeEBO(indices,sizeof(indices),4);
    return new_shape;

}

BasicShape GetTextureRectangle (VAOStruct vao, glm::vec3 location, float width, float height)
{
    float vertices[] 
    {
        location.x, location.y, 0.0, 0.0, 0.0,
        location.x+width, location.y, 0.0, 1.0, 0.0,
        location.x+width, location.y+height,0.0, 1.0,1.0,
        location.x, location.y+height, 0.0, 0.0, 1.0
    };
    BasicShape new_shape;
    new_shape.Initialize(vao,vertices,sizeof(vertices),4,GL_TRIANGLE_FAN);
    unsigned int indices[] = {0,1,2,3};
    new_shape.InitializeEBO(indices,sizeof(indices),4);
    return new_shape;

}

BasicShape GetCircle (VAOStruct vao, float radius, int points, glm::vec3 location) {
    float circ_dat[points*3]{};
    float angle = 0.0;
    for(int i = 0; i<points; i++) {
        circ_dat[i*3] = location.x + radius*cos(glm::radians(angle));
        circ_dat[i*3+1] = location.y + radius*sin(glm::radians(angle));
        circ_dat[i*3+2] = 0.0f;
        angle+=360.0f/points;
    }
    
    BasicShape new_shape;
    new_shape.Initialize(vao,circ_dat,sizeof(circ_dat),points,GL_TRIANGLE_FAN);

    //for use with the EBO
    unsigned int indices[points]{};
    for(int i = 0; i<points; i++)
    {
        indices[i] = i;
    }
    //set the ebo for the shape here
    new_shape.InitializeEBO(indices,sizeof(indices),points);
    return new_shape;
}

BasicShape GetTextureCircle (VAOStruct vao, float radius, int points, glm::vec3 location) {
    float circ_dat[points*5]{};
    float angle = 0.0;
    for(int i = 0; i<points; i++) {
        circ_dat[i*5] = location.x + radius*cos(glm::radians(angle));
        circ_dat[i*5+1] = location.y + radius*sin(glm::radians(angle));
        circ_dat[i*5+2] = 0.0f;
        circ_dat[i*5+3] = 0.5 + (1+ cos(glm::radians(angle))/2);
        circ_dat[i*5+4] = 0.5 + (1+ sin(glm::radians(angle))/2);
        angle+=360.0f/points;
    }
    
    BasicShape new_shape;
    new_shape.Initialize(vao,circ_dat,sizeof(circ_dat),points,GL_TRIANGLE_FAN);

    //for use with the EBO
    unsigned int indices[points]{};
    for(int i = 0; i<points; i++)
    {
        indices[i] = i;
    }
    //set the ebo for the shape here
    new_shape.InitializeEBO(indices,sizeof(indices),points);
    return new_shape;
}


BasicShape GetHull (VAOStruct vao, glm::vec3 lower_left, float width, float height)
{
    
float data[] = {
        lower_left.x, lower_left.y, 0.0f,
        lower_left.x+width, lower_left.y, 0.0f,
        lower_left.x+width, lower_left.y+height, 0.0f,
        lower_left.x+0.9f*width, lower_left.y+height+0.05f,0.0f,
        lower_left.x+0.8f*width, lower_left.y+height+0.09f,0.0f,
        lower_left.x+0.7f*width, lower_left.y+height+0.11f,0.0f,
        lower_left.x+0.6f*width, lower_left.y+height+0.13f,0.0f,
        lower_left.x+0.5f*width, lower_left.y+height+0.15f,0.0f,
        lower_left.x+0.4f*width, lower_left.y+height+0.13f,0.0f,
        lower_left.x+0.3f*width, lower_left.y+height+0.11f,0.0f,
        lower_left.x+0.2f*width, lower_left.y+height+0.09f,0.0f,
        lower_left.x+0.1f*width, lower_left.y+height+0.05f,0.0f,
        lower_left.x, lower_left.y+height, 0.0f
    };
    BasicShape new_shape;
    new_shape.Initialize(vao,data,sizeof(data),sizeof(data)/sizeof(float)/3,GL_TRIANGLE_FAN);
    unsigned int indices[sizeof(data)/sizeof(float)/3];
    for (int i = 0; i<sizeof(data)/sizeof(float)/3; i++){indices[i] = i;}
    new_shape.InitializeEBO(indices,sizeof(indices),sizeof(data)/sizeof(float)/3);
    return new_shape;

}

BasicShape GetStars (VAOStruct vao, int num_stars) {
    float data[num_stars*3] = {0.0f};
    for (int i = 0; i < num_stars; i+=3) {
        data[i] = 1.0f*(((rand() % 1000)-500)/500.0f);
        data[i+1] = 1.0f*(((rand()%1000)-500)/500.0f);
    }

    BasicShape new_shape;
    new_shape.Initialize(vao,data,sizeof(data),num_stars,GL_POINTS);
    return new_shape;
}
