//
//  main.cpp
//  COMP371A2
//
//  Created by Zhao Rui on 2018-02-23.
//  Copyright © 2018 Zhao Rui. All rights reserved.
//

#include <iostream>
#include "ShaderProg.h"



// GLEW
#define GLEW_STATIC
#include </usr/local/Cellar/glew/2.1.0/include/GL/glew.h>

//GLFW
#include </usr/local/Cellar/glfw-master/include/GLFW/glfw3.h>

//glm
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/glm.hpp>
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/gtc/type_ptr.hpp>
using namespace std;

unsigned int WIDTH=800, HEIGHT=800;
float moveOnX=0,moveOnZ=0;
const float minMove=-30.0f,maxMove=30.0f;
float userScale=1.0f;
float userRotateOnY=0.0f;
float userRotateOnZ=0.0f;
//glm::vec3 rotateOri1=glm::vec3(0.0f,1.0f,0.0f);
//glm::vec3 rotateOri2=glm::vec3(0.0f,0.0f,1.0f);
float cameraRotate=10.0f;

//horse positions
glm::vec3 bodyPosition=glm::vec3(0.0f,1.7f,0.0f);//body
glm::mat4 model_body;
glm::mat4 model_neck;
glm::mat4 model_fru;
glm::mat4 model_flu;
glm::mat4 model_bru;
glm::mat4 model_blu;
glm::mat4 model_base=glm::mat4(1.0f);
ShaderProg horseShader;

//camera attributes
float yaw=-90.0f;
float pitch=0.0f;
float lastX=WIDTH/2.0f;//camera first set to the origin
float lastY=HEIGHT/2.0f;//camera first set to the origin
float fov=45.0f;//perspective angle
bool leftMouseButton=false, middleMouseButton=false, rightMouseButton=false;//varibles for detecting the mouse button action
bool firstMouse=true;

//light
glm::vec3 lightPos(0.0f,21.7f,0.0f);//20 unit above the horse


// ---- VIEW MATRIX global variables -----
glm::vec3 c_pos = glm::vec3(0.0f,0.0f, 30.0f); // camera position
glm::vec3 c_dir = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)); // camera direction
glm::vec3 c_up = glm::vec3(0, 1, 0); // tell the camera which way is 'up'
glm::mat4 view;
glm::mat4 projection;
glm::mat4 model;

//------------draw horse functions---------------
void body();
void frontLeftUpperLeg();
void frontLeftLowerLeg();
void frontRightUpperLeg();
void frontRightLowerLeg();
void backLeftUpperLeg();
void backLeftLowerLeg();
void backRightUpperLeg();
void backRightLowerLeg();
void neck();
void head();

void window_size_callback(GLFWwindow* window, int width, int height)
{
    //projection=glm::perspective(fov, (float)width/(float)height, 0.1f, 100.0f);
    WIDTH=width;
    HEIGHT=height;
    int width_,height_;
    glfwGetFramebufferSize(window, &width_, &height_);
    glViewport(0, 0, width_, height_);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key==GLFW_KEY_LEFT)//world orientation to right
    {
        c_pos-=glm::normalize(glm::cross(c_dir, c_up));
        //updateView();
    }
    if(key==GLFW_KEY_RIGHT)//world orientation to left
    {
        c_pos+=glm::normalize(glm::cross(c_dir, c_up));
        //updateView();
    }
    if (key == GLFW_KEY_DOWN)//world orientation Ry
    {
        c_pos.y+=1;
        //updateView();
    }
    if (key == GLFW_KEY_UP)//world orientation -Ry
    {
        c_pos.y-=1;
        //updateView();
    }
    if(key==GLFW_KEY_TAB)//reset to the initial world position and orientation.because I'm using Mac, which doesn't have "Home" button, I used "tab" instead
    {
        c_pos = glm::vec3(0.0f,0.0f, 30.0f);
        c_dir = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));  // camera direction
        c_up = glm::vec3(0, 1, 0);
        yaw=-90.0f;
        pitch=0.0f;
        lastX=WIDTH/2.0f;//camera first set to the origin
        lastY=HEIGHT/2.0f;//camera first set to the origin
        fov=45.0f;
        moveOnX=0;
        moveOnZ=0;
        userScale=1.0f;
        userRotateOnY=0.0f;
        userRotateOnZ=0.0f;
        model=glm::mat4(1.0f);
        
        //updateView();
    }
    if(key==GLFW_KEY_SPACE&& action == GLFW_PRESS)//randomly change the position of the horse on the grid
    {
        //generating random location in the grid
        moveOnX=minMove + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxMove-minMove)));
        moveOnZ=minMove + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxMove-minMove)));
    }
    if(key==GLFW_KEY_U&&action==GLFW_PRESS)//scale up
    {
        userScale+=0.5;
    }
    if(key==GLFW_KEY_J&&action==GLFW_PRESS)//scale down
    {
        userScale-=0.5;
    }
    if(key==GLFW_KEY_A&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT)//move to left by 1 unit grid
        {
            //uppercase
            moveOnX+=-1.0f;
        }
        else //rotate to left by 5 degrees related to y axis
        {
            //lowercase
            userRotateOnY+=5.0f;
        }
    }
    if(key==GLFW_KEY_D&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {////move to right by 1 unit grid
            //uppercase
            moveOnX+=1.0f;
        }
        else//rotate to left by 5 degrees related to y axis
        {
            //lowercase
            userRotateOnY-=5.0f;
        }
    }
    if(key==GLFW_KEY_W&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {//move up by 1 unit grid
            //uppercase
            moveOnZ+=-1.0f;
        }
        else {//rise head by 5 degrees
            //lowercase
            userRotateOnZ-=5.0f;
        }
    }
    if(key==GLFW_KEY_S&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {//move down by 1 unit grid
            //uppercase
            moveOnZ+=1.0f;
        }
        else {//rise rear by 5 degrees
            //lowercase
            userRotateOnZ+=5.0f;
        }
    }
    if(key==GLFW_KEY_P&&action==GLFW_PRESS)
    {
        glPointSize(10.0f);//make the points more visible
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);//point mode
    }
    if(key==GLFW_KEY_L&&action==GLFW_PRESS)
    {
        glLineWidth(10.0f);//make the lines more visible
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//point mode
    }
    
    if(key==GLFW_KEY_T&&action==GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//wireframe mode
    }
}

//call back funtion for mouse button and movement
void mouse_button_callback(GLFWwindow* window, int key, int action, int mode)
{
    if(key==GLFW_MOUSE_BUTTON_RIGHT&&action==GLFW_PRESS)
    {
        rightMouseButton=true;
    }
    if(key==GLFW_MOUSE_BUTTON_MIDDLE&&action==GLFW_PRESS)
    {
        middleMouseButton=true;
    }
    if(key==GLFW_MOUSE_BUTTON_LEFT&&action==GLFW_PRESS)
    {
        leftMouseButton=true;
    }
    if(key==GLFW_MOUSE_BUTTON_RIGHT&&action==GLFW_RELEASE)
    {
        rightMouseButton=false;
    }
    if(key==GLFW_MOUSE_BUTTON_MIDDLE&&action==GLFW_RELEASE)
    {
        middleMouseButton=false;
    }
    if(key==GLFW_MOUSE_BUTTON_LEFT&&action==GLFW_RELEASE)
    {
        leftMouseButton=false;
    }
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)//keep track of the position of the cursor
{
    if(firstMouse)
    {
        //cout<<"here"<<endl;
        lastX=xPos;
        lastY=yPos;
        firstMouse=false;
    }
    
    
    float xOffset=xPos-lastX;
    float yOffset=lastY-yPos;
    lastX=xPos;
    lastY=yPos;
    
    float sensitivity=0.05f;
    xOffset*=sensitivity;
    yOffset*=sensitivity;
    
    if(rightMouseButton)//yaw
    {
        yaw+=xOffset;
    }
    if(middleMouseButton)//pitch
    {
        pitch+=yOffset;
        if(pitch>89.0f)
        {
            pitch=89.0f;
        }
        if(pitch<-89.0f)
        {
            pitch=-89.0f;
        }
    }
    
    if(leftMouseButton)//zoom in and out by adjusting the fov degree
    {
        //cout<<"here"<<endl;
        if(fov>=44.0f&&fov<=45.0f)
            fov-=yOffset*0.1;
        if(fov<=44.0f)
            fov=44.0f;
        if(fov>=45.0f)
            fov=45.0f;
    }
    
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    c_dir = glm::normalize(front);
}



int main() {
    //initialize glfw
    if(!glfwInit())
    {
        cout<<"Failed to initialize glfw."<<endl;
        return EXIT_FAILURE;
    }
    
    //glfw configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //create a window
    GLFWwindow *window=glfwCreateWindow(WIDTH,HEIGHT, "assignment1", nullptr, nullptr);
    if(!window)
    {
        cout<<"Failed to open a glfw window."<<endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glewExperimental=GL_TRUE;
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    /*
     //configure viewport
     int screenWidth, screenHeight;
     glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
     glViewport(0, 0, screenWidth, screenHeight);
     */
    
    //initialize glew
    if(GLEW_OK!=glewInit())
    {
        cout<<"Failed to initialize glew."<<endl;
        return EXIT_FAILURE;
    }
    
    //enable z-buffer
    glEnable(GL_DEPTH_TEST);
    
    
    //vertices for the ground square
    float square[]={
        
        -0.5f,-0.5f,0.0f,//left bottom
        0.5f,-0.5f,0.0f,//right bottom
        0.5f,0.5f,0.0f,//right top
        -0.5f,0.5f,0.0f,//left top
    };
    
    float coordinate[]={
        
        0.0f,0.0f,0.0f,//x axis
        1.0f,0.0f,0.0f,
        
        
        0.0f,0.0f,0.0f,//y axis
        0.0f,1.0f,0.0f,
        
        0.0f,0.0f,0.0f,//z axis
        0.0f,0.0f,1.0f
        
        
    };
    
    ShaderProg groundShader("squarevs.vs","squarefs.fs");//shader program for ground
    horseShader=ShaderProg("horsevs.vs","horsefs.fs");//shader program for horse
    ShaderProg coordinateShader("squarevs.vs","squarefs.fs");//shader program for coordinate
    ShaderProg lightShader("lightvs.vs","lightfs.fs");//light shader
    
    
    //vertices for a cube
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    
    
    //VAO
    GLuint VAOs[4], VBOs[3];//four VAOs and VBOs, one for the ground, one for the horse
    glGenVertexArrays(4,VAOs);
    glGenBuffers(3,VBOs);
    
    
    //ground
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(square),square,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    
    //coordinates
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(coordinate),coordinate,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    
    //horse
    //---------horse part--------------
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    //---------normals-----------------
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    //light
    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    
    
    //game loop
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        
        
        //view matrix
        view=glm::lookAt(c_pos, c_pos+c_dir, c_up);
        
        
        //projection matrix
        projection=glm::mat4(1.0f);
        projection=glm::perspective(fov, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        
        
        //initialize model matrix
        model=glm::mat4(1.0f);
        
        
        //draw ground
        glBindVertexArray(VAOs[0]);
        groundShader.use();
        groundShader.setMat4("view", view);
        groundShader.setMat4("projection", projection);
        for(int row=0;row<100;row++)
        {
            for(int column=0;column<100;column++)
            {
                model=glm::mat4(1.0f);
                model=glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f,0.0f,0.0f));
                model=glm::translate(model, glm::vec3(-49.5f+(float)column,-49.5f+(float)row,0.0f));
                //we want the center of the grid at the world origin, therefore, the left bottom point should be at (-50,-50,0), consequently, the first offset is -50-(-0.5)=-49.5, same for the row
                groundShader.setVec4("lineColor", 1.0f,1.0f,1.0f,1.0f);
                groundShader.setMat4("model", model);
                glDrawArrays(GL_LINE_LOOP, 0, 4);
            }
        }
        
        
        //draw coordinates
        glBindVertexArray(VAOs[1]);
        coordinateShader.use();
        coordinateShader.setMat4("view", view);
        coordinateShader.setMat4("projection", projection);
        for(int i=0;i<5;i++)
        {
            model=glm::mat4(1.0f);
            coordinateShader.setVec4("lineColor", 1.0, 0.0f, 0.0f, 1.0f);
            model=glm::translate(model, glm::vec3(0.0f+(float)i,0.0f+0.02,0.0f));//give the transform a little offset on Y axis, so it wont be hiden from the grid
            coordinateShader.setMat4("model", model);
            glDrawArrays(GL_LINES, 0, 2);
        }
        for(int i=0;i<5;i++)
        {
            model=glm::mat4(1.0f);
            coordinateShader.setVec4("lineColor", 0.0, 1.0f, 0.0f, 1.0f);
            model=glm::translate(model, glm::vec3(0.0f,0.0f+(float)i+0.02,0.0f));//give the transform a little offset on Y axis, so it wont be hiden from the grid
            coordinateShader.setMat4("model", model);
            glDrawArrays(GL_LINES, 2, 2);
        }
        for(int i=0;i<5;i++)
        {
            model=glm::mat4(1.0f);
            coordinateShader.setVec4("lineColor", 0.0, 0.0f, 1.0f, 1.0f);
            model=glm::translate(model, glm::vec3(0.0f,0.0f+0.02,0.0f+(float)i));//give the transform a little offset on Y axis, so it wont be hiden from the grid
            coordinateShader.setMat4("model", model);
            glDrawArrays(GL_LINES, 4, 2);
        }
        
        //draw light
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.3f)); // a smaller cube
        lightShader.setMat4("model", model);
        
        glBindVertexArray(VAOs[3]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        //draw horse
        glBindVertexArray(VAOs[2]);
        horseShader.use();
        horseShader.setMat4("view", view);
        horseShader.setMat4("projection", projection);
        horseShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        horseShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        horseShader.setVec3("lightPos", lightPos);
        horseShader.setVec3("viewPos", c_pos);
        //--------------------------------------------------

        
        body();
        frontLeftUpperLeg();
        frontLeftLowerLeg();
        frontRightUpperLeg();
        frontRightLowerLeg();
        backLeftUpperLeg();
        backLeftLowerLeg();
        backRightUpperLeg();
        backRightLowerLeg();
        neck();
        head();
        
        
        glfwSwapBuffers(window);
        //glfwPollEvents();
    }
    
    glDeleteVertexArrays(3,VAOs);
    glDeleteBuffers(3,VBOs);
    
    glfwTerminate();
    return 0;
}

void body()
{
    model_body=glm::scale(model_base, glm::vec3(4.0f*userScale,1.5f*userScale,1.5f*userScale));
    model_body=glm::translate(model_body, glm::vec3(bodyPosition[0]+moveOnX,bodyPosition[1],bodyPosition[2]+moveOnZ));
    model_body=glm::rotate(model_body, glm::radians(userRotateOnZ), glm::vec3(0.0f,0.0f,1.0f));
    model_body=glm::rotate(model_body, glm::radians(userRotateOnY), glm::vec3(0.0f,1.0f,0.0f));
    horseShader.setMat4("model", model_body);
    horseShader.setVec3("partColor", glm::vec3(0.2f,0.2f,0.1f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void frontLeftUpperLeg()
{
    model_flu = glm::translate(model_body, glm::vec3(-0.375f, -0.67f, -0.25f));
    model_flu = glm::scale(model_flu, glm::vec3(0.125f,0.67f , 0.25f));
    horseShader.setMat4("model", model_flu);
    horseShader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void frontLeftLowerLeg()
{
    glm::mat4 model_fll = glm::translate(model_flu, glm::vec3(0, -1, 0));
    horseShader.setMat4("model", model_fll);
    horseShader.setVec3("partColor", glm::vec3(0.2f,0.6f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void frontRightUpperLeg()
{
    model_fru = glm::translate(model_body, glm::vec3(-0.375f, -0.67f, 0.25f));
    model_fru = glm::scale(model_fru, glm::vec3(0.125f,0.67f , 0.25f));
    horseShader.setMat4("model", model_fru);
    horseShader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void frontRightLowerLeg()
{
    glm::mat4 model_frl = glm::translate(model_fru, glm::vec3(0, -1, 0));
    horseShader.setMat4("model", model_frl);
    horseShader.setVec3("partColor", glm::vec3(0.2f,0.6f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void backLeftUpperLeg()
{
    model_blu = glm::translate(model_body, glm::vec3(0.375f, -0.67f, -0.25f));
    model_blu = glm::scale(model_blu, glm::vec3(0.125f,0.67f , 0.25f));
    horseShader.setMat4("model", model_blu);
    horseShader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void backLeftLowerLeg()
{
    glm::mat4 model_bll = glm::translate(model_blu, glm::vec3(0, -1, 0));
    horseShader.setMat4("model",model_bll);
    horseShader.setVec3("partColor", glm::vec3(0.2f,0.6f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void backRightUpperLeg()
{
    model_bru=glm::translate(model_body, glm::vec3(0.375f, -0.67f, 0.25f));
    model_bru = glm::scale(model_bru, glm::vec3(0.125f,0.67f , 0.25f));
    horseShader.setMat4("model", model_bru);
    horseShader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void backRightLowerLeg()
{
    glm::mat4 model_brl = glm::translate(model_bru, glm::vec3(0, -1, 0));
    horseShader.setMat4("model", model_brl);
    horseShader.setVec3("partColor", glm::vec3(0.2f,0.6f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void neck()
{
    model_neck=glm::translate(model_body, glm::vec3(-0.6, 0.18, 0.0));
    model_neck=glm::rotate(model_neck, glm::radians(-53.0f), glm::vec3(0.0, 0.0, 1.0));
    model_neck=glm::scale(model_neck,glm::vec3(0.96, 0.46, 0.5));
    horseShader.setMat4("model", model_neck);
    horseShader.setVec3("partColor", glm::vec3(0.4f,0.2f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void head()
{
    glm::mat4 model_head = glm::translate(model_neck, glm::vec3(-0.2, -0.5, 0.0));
    model_head = glm::rotate(model_head, glm::radians(16.63f), glm::vec3(0.0, 0.0, 1.0));
    model_head = glm::scale(model_head, glm::vec3(0.34, 1.65, 0.5));
    horseShader.setMat4("model", model_head);
    horseShader.setVec3("partColor", glm::vec3(0.4f,0.3f,0.3f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

