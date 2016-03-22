/*
 *  ARWrapperNativeExample.cpp
 *  ARToolKit for Android
 *
 *  Disclaimer: IMPORTANT:  This Daqri software is supplied to you by Daqri
 *  LLC ("Daqri") in consideration of your agreement to the following
 *  terms, and your use, installation, modification or redistribution of
 *  this Daqri software constitutes acceptance of these terms.  If you do
 *  not agree with these terms, please do not use, install, modify or
 *  redistribute this Daqri software.
 *
 *  In consideration of your agreement to abide by the following terms, and
 *  subject to these terms, Daqri grants you a personal, non-exclusive
 *  license, under Daqri's copyrights in this original Daqri software (the
 *  "Daqri Software"), to use, reproduce, modify and redistribute the Daqri
 *  Software, with or without modifications, in source and/or binary forms;
 *  provided that if you redistribute the Daqri Software in its entirety and
 *  without modifications, you must retain this notice and the following
 *  text and disclaimers in all such redistributions of the Daqri Software.
 *  Neither the name, trademarks, service marks or logos of Daqri LLC may
 *  be used to endorse or promote products derived from the Daqri Software
 *  without specific prior written permission from Daqri.  Except as
 *  expressly stated in this notice, no other rights or licenses, express or
 *  implied, are granted by Daqri herein, including but not limited to any
 *  patent rights that may be infringed by your derivative works or by other
 *  works in which the Daqri Software may be incorporated.
 *
 *  The Daqri Software is provided by Daqri on an "AS IS" basis.  DAQRI
 *  MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 *  THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE, REGARDING THE DAQRI SOFTWARE OR ITS USE AND
 *  OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 *
 *  IN NO EVENT SHALL DAQRI BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 *  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 *  MODIFICATION AND/OR DISTRIBUTION OF THE DAQRI SOFTWARE, HOWEVER CAUSED
 *  AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 *  STRICT LIABILITY OR OTHERWISE, EVEN IF DAQRI HAS BEEN ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  Copyright 2015 Daqri LLC. All Rights Reserved.
 *  Copyright 2011-2015 ARToolworks, Inc. All Rights Reserved.
 *
 *  Author(s): Julian Looser, Philip Lamb
 */

#include <AR/gsub_es.h>
#include <Eden/glm.h>
#include <jni.h>
#include <ARWrapper/ARToolKitWrapperExportedAPI.h>
#include <unistd.h> // chdir()
#include <android/log.h>

#include <time.h>
#include <string.h>
#include <stdio.h>

// Utility preprocessor directive so only one change needed if Java class name changes
#define JNIFUNCTION_DEMO(sig) Java_org_artoolkit_ar_samples_ARSimpleNative_SimpleNativeRenderer_##sig

extern "C" {
	JNIEXPORT void JNICALL JNIFUNCTION_DEMO(demoInitialise(JNIEnv* env, jobject object));
	JNIEXPORT void JNICALL JNIFUNCTION_DEMO(demoShutdown(JNIEnv* env, jobject object));
	JNIEXPORT void JNICALL JNIFUNCTION_DEMO(demoSurfaceCreated(JNIEnv* env, jobject object));
	JNIEXPORT void JNICALL JNIFUNCTION_DEMO(demoSurfaceChanged(JNIEnv* env, jobject object, jint w, jint h));
	JNIEXPORT void JNICALL JNIFUNCTION_DEMO(demoDrawFrame(JNIEnv* env, jobject obj));	
    JNIEXPORT int JNICALL JNIFUNCTION_DEMO(habreImpactadoNat(JNIEnv* env, jobject obj));
};

static int in;
static int max_jugadores = 8;
static int markerID[8];

static float transX[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static float transY[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static float transZ[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static int detectado[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
static double tiempoTolerancia[8] = {100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0};
static double tiempoAnterior[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static float markerSize = 100.0;


JNIEXPORT void JNICALL JNIFUNCTION_DEMO(demoInitialise(JNIEnv* env, jobject object)) {
    arwSetPatternDetectionMode(AR_MATRIX_CODE_DETECTION);
    arwSetMatrixCodeType(AR_MATRIX_CODE_3x3_PARITY65);

	char buf[25];
    for(in = 0; in<max_jugadores; in++){
        sprintf(buf, "single_barcode;%d;80", in);
        markerID[in] = arwAddMarker(buf);
    }
}

JNIEXPORT void JNICALL JNIFUNCTION_DEMO(demoShutdown(JNIEnv* env, jobject object)) {
}

JNIEXPORT void JNICALL JNIFUNCTION_DEMO(demoSurfaceCreated(JNIEnv* env, jobject object)) {
	glStateCacheFlush(); // Make sure we don't hold outdated OpenGL state.
}

JNIEXPORT void JNICALL JNIFUNCTION_DEMO(demoSurfaceChanged(JNIEnv* env, jobject object, jint w, jint h)) {
	// glViewport(0, 0, w, h) has already been set.
}


JNIEXPORT int JNICALL JNIFUNCTION_DEMO(habreImpactadoNat(JNIEnv* env, jobject object)) {
    int ventana, i;

    for(i=0; i<8; i++){
        if(detectado[i] >= 0){
            if(transZ[i] == markerSize){   
                ventana = 100; 
            }
            else{
                ventana = 100 - (transZ[i] / markerSize) * 10;
            }

            if( (transX[i] >= 0 - ventana && transX[i] <= 0 + ventana)   &&  (transY[i] >= 0 - ventana && transY[i] <= 0 + ventana)){
                return detectado[i];
            }
        }
    }
    return -1;
}

static void dibujaDiana(float size, float x, float y, float z, GLubyte r, GLubyte g, GLubyte b){
    int i;
    GLubyte alp = 180;
    const GLfloat vertices [34][3] = {
        /* +z */ {0.0f, 0.5f, 0.0f}, {0.06f, 0.49f, 0.0f}, {0.12f, 0.48f, 0.0f}, {0.18f, 0.46f, 0.0f}, {0.25f, 0.43f, 0.0f} , {0.31f, 0.39f, 0.0f}, {0.37f, 0.33f, 0.0f}, {0.43f, 0.24f, 0.0f}, {0.5f, 0.0f, 0.0f}, 
                 {0.43f, -0.24f, 0.0f}, {0.37f, -0.33f, 0.0f}, {0.31f, -0.39f, 0.0f}, {0.25f, -0.43f, 0.0f}, {0.18f, -0.46f, 0.0f} , {0.12f, -0.48f, 0.0f}, {0.06f, -0.49f, 0.0f}, {0.0f, -0.5f, 0.0f}, 
                 {0.0f, -0.5f, 0.0f}, {-0.06f, -0.49f, 0.0f}, {-0.12f, -0.48f, 0.0f}, {-0.18f, -0.46f, 0.0f}, {-0.25f, -0.43f, 0.0f} , {-0.31f, -0.39f, 0.0f}, {-0.37f, -0.33f, 0.0f}, {-0.43f, -0.24f, 0.0f}, {-0.5f, 0.0f, 0.0f},
                 {-0.43f, 0.24f, 0.0f}, {-0.37f, 0.33f, 0.0f}, {-0.31f, 0.39f, 0.0f}, {-0.25f, 0.43f, 0.0f}, {-0.18f, 0.46f, 0.0f} , {-0.12f, 0.48f, 0.0f}, {-0.06f, 0.49f, 0.0f}, {0.0f, 0.5f, 0.0f} };
    const GLubyte vertex_colors [34][4] = {
        {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, 
        {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp},
        {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp},
        {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp}, {r, g, b, alp},
        {r, g, b, alp}, {r, g, b, alp} };
    const GLushort face [34] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};


    const GLfloat verticesC [2][3] = {
        /* +z */ {-0.5f, 0.0f, 0.02f}, {0.5f, 0.0f, 0.02f} };
    const GLfloat verticesC2 [2][3] = {
        {0.0f, -0.5f, 0.02f}, {0.0f, 0.5f, 0.02f}   };
    const GLushort faceC [2] = {0, 1};
    /*
    const GLfloat tex_vertices [4][2] = {
        {0.5f, 0.5f}, {0.5f, -0.5f}, {-0.5f, -0.5f}, {-0.5f, 0.5f}
    };

    const GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );
    const GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    const GLuint Texture = loadDDS("diana.DDS");
    const GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    glGenTextures(1, &Texture);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    */

    glPushMatrix(); // Save world coordinate system.
    glTranslatef(x, y, z);
    glScalef(size, size, size);
    glStateCacheDisableLighting();
    glStateCacheDisableTex2D();
    glStateCacheDisableBlend();
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, vertex_colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    /*
    glTexCoordPointer(2, GL_FLOAT , 0, tex_vertices);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    */



    glStateCacheEnableClientStateVertexArray();
    glEnableClientState(GL_COLOR_ARRAY);

    glDrawElements(GL_TRIANGLE_FAN, 34, GL_UNSIGNED_SHORT, &(face[0]));
    
    glDisableClientState(GL_COLOR_ARRAY);
    glColor4ub(0, 0, 0, 255);
    glDrawElements(GL_LINE_LOOP, 34, GL_UNSIGNED_SHORT, &(face[0]));
    
    glPopMatrix();    // Restore world coordinate system.

    /////////////////////////////////////////////////////////////////////////////////CRUZ

    
    glPushMatrix(); // Save world coordinate system.
    glTranslatef(x, y, z);
    glScalef(size, size, size);
    glStateCacheDisableLighting();
    glStateCacheDisableTex2D();
    glStateCacheDisableBlend();
    glVertexPointer(3, GL_FLOAT, 0, verticesC);
    glStateCacheEnableClientStateVertexArray();
    glColor4ub(0, 0, 0, 255);
    glDrawElements(GL_LINE_LOOP, 2, GL_UNSIGNED_SHORT, &(faceC[0]));
    
    glVertexPointer(3, GL_FLOAT, 0, verticesC2);
    glStateCacheEnableClientStateVertexArray();
    glColor4ub(0, 0, 0, 255);
    glDrawElements(GL_LINE_LOOP, 2, GL_UNSIGNED_SHORT, &(faceC[0]));

    glPopMatrix();    // Restore world coordinate system.



    glPushMatrix(); // Save world coordinate system.
    glTranslatef(x+0.03, y+0.03, z);
    glScalef(size, size, size);
    glStateCacheDisableLighting();
    glStateCacheDisableTex2D();
    glStateCacheDisableBlend();
    glVertexPointer(3, GL_FLOAT, 0, verticesC);
    glStateCacheEnableClientStateVertexArray();
    glColor4ub(255, 0, 0, 255);
    glDrawElements(GL_LINE_LOOP, 2, GL_UNSIGNED_SHORT, &(faceC[0]));
    
    glVertexPointer(3, GL_FLOAT, 0, verticesC2);
    glStateCacheEnableClientStateVertexArray();
    glColor4ub(255, 0, 0, 255);
    glDrawElements(GL_LINE_LOOP, 2, GL_UNSIGNED_SHORT, &(faceC[0]));

    glPopMatrix();    // Restore world coordinate system.



}

static void dibujaMira(float size){
    int i;
    const GLfloat vertices [2][3] = {
        /* +z */ {-1.0f, 0.0f, 0.0f}, {1.0f, -0.0f, 0.0f} };
    const GLfloat vertices2 [2][3] = {
        {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}   };
    const GLushort face [2] = {0, 1};
    glPushMatrix(); // Save world coordinate system.
    glTranslatef(0, 0, -10);
    glScalef(size, size, size);
    glStateCacheDisableLighting();
    glStateCacheDisableTex2D();
    glStateCacheDisableBlend();
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glStateCacheEnableClientStateVertexArray();
    glColor4ub(0, 0, 0, 255);
    glDrawElements(GL_LINE_LOOP, 2, GL_UNSIGNED_SHORT, &(face[0]));
    
    glVertexPointer(3, GL_FLOAT, 0, vertices2);
    glStateCacheEnableClientStateVertexArray();
    glColor4ub(0, 0, 0, 255);
    glDrawElements(GL_LINE_LOOP, 2, GL_UNSIGNED_SHORT, &(face[0]));

    glPopMatrix();    // Restore world coordinate system.



    glPushMatrix(); // Save world coordinate system.
    glTranslatef(0.02, 0.02, -10);
    glScalef(size, size, size);
    glStateCacheDisableLighting();
    glStateCacheDisableTex2D();
    glStateCacheDisableBlend();
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glStateCacheEnableClientStateVertexArray();
    glColor4ub(255, 255, 255, 255);
    glDrawElements(GL_LINE_LOOP, 2, GL_UNSIGNED_SHORT, &(face[0]));
    
    glVertexPointer(3, GL_FLOAT, 0, vertices2);
    glStateCacheEnableClientStateVertexArray();
    glColor4ub(255, 255, 255, 255);
    glDrawElements(GL_LINE_LOOP, 2, GL_UNSIGNED_SHORT, &(face[0]));

    glPopMatrix();    // Restore world coordinate system.
}

static void drawCube(float size, float x, float y, float z)
{
    // Colour cube data.
    int i;
    const GLfloat cube_vertices [8][3] = {
        /* +z */ {0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
        /* -z */ {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f} };
    const GLubyte cube_vertex_colors [8][4] = {
        {255, 255, 255, 255}, {255, 255, 0, 255}, {0, 255, 0, 255}, {0, 255, 255, 255},
        {255, 0, 255, 255}, {255, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 255, 255} };
    const GLushort cube_faces [6][4] = { /* ccw-winding */
        /* +z */ {3, 2, 1, 0}, /* -y */ {2, 3, 7, 6}, /* +y */ {0, 1, 5, 4},
        /* -x */ {3, 0, 4, 7}, /* +x */ {1, 2, 6, 5}, /* -z */ {4, 5, 6, 7} };
    
    glPushMatrix(); // Save world coordinate system.
    glTranslatef(x, y, z);
    glScalef(size, size, size);
    glStateCacheDisableLighting();
    glStateCacheDisableTex2D();
    glStateCacheDisableBlend();
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, cube_vertex_colors);
    glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
    glStateCacheEnableClientStateVertexArray();
    glEnableClientState(GL_COLOR_ARRAY);
    for (i = 0; i < 6; i++) {
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, &(cube_faces[i][0]));
    }
    glDisableClientState(GL_COLOR_ARRAY);
    glColor4ub(0, 0, 0, 255);
    for (i = 0; i < 6; i++) {
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, &(cube_faces[i][0]));
    }
    glPopMatrix();    // Restore world coordinate system.
}

JNIEXPORT void JNICALL JNIFUNCTION_DEMO(demoDrawFrame(JNIEnv* env, jobject obj)) {
	
    double start, end, delta;
    FILE* file;
    FILE* file2;
    char tiempo[30], cadena[100];

    struct timespec res;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    // Set the projection matrix to that provided by ARToolKit.
	float proj[16];



	arwGetProjectionMatrix(proj);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(proj);
	glMatrixMode(GL_MODELVIEW);
	
    /*
    file2 = fopen("/sdcard/Matrices.txt", "a");

    sprintf(cadena,"Mat Proy \n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n", proj[0], proj[4], proj[8], proj[12], 
                                                                                        proj[1], proj[5], proj[9], proj[13],
                                                                                        proj[2], proj[6], proj[10], proj[14],
                                                                                        proj[3], proj[7], proj[11], proj[15]);
    if (file2 != NULL){
        fputs(cadena, file2);
        fflush(file2);
        fclose(file2);
    }
    */

	glStateCacheEnableDepthTest();
	glStateCacheDisableLighting();	
	glStateCacheDisableTex2D();	

    glLoadIdentity();
    dibujaMira(10.0f);


    for(in = 0; in<4; in++){

        clock_gettime(CLOCK_REALTIME, &res);
        start = 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;

        if (arwQueryMarkerVisibility(markerID[in])) {

            float trans[16];
            arwQueryMarkerTransformation(markerID[in], trans);

            detectado[in] = 1;

            transX[in] = trans[12];
            transY[in] = trans[13];
            transZ[in] = trans[14];

            //trans[12] = 0.0;
            //trans[13] = 0.0;
            //trans[14] = -250.0;        //250 3.8cm   //500 1.8cm
                                    //100 completa  8.3cm

                                    //size actual = 50 x 50    = 8.3 x 8.3

                                //50 = 1.9cm
                                //100 = 3.8cm
                                
                                //Un punto equivale a 0.038 cm

            //Conclusion: Por cada 100 unidades de translacion en Z, se divide el tamaño en un punto
            //Ej: Si la translacion es 500, el tamanio del objeto se divide entre 5

            //Ahora que el objeto de realidad aumentada cambio a ser circular, se debe utilizar una
            //formula para obtener un area de impacto circular

            //Nuevo tamanio: 150


            //if(trans[14] <= 100)
            /*

            size 100 Z 100   =   100x100 de ventana

            size 100 Z 250   =   75x75 de ventana

            size 100 Z 500   =   50x50 de ventana


            size 50 Z 50    =    100x100 de ventana

            size 50 Z 250   =    50x50 de ventana

            
            if(Z == size){   ventana = 100; }
            else{
            100 - (Z / size) * 10 = ventana }

            if( (trans[x] >= 0 - ventana && trans[x] <= 0 + ventana)   &&  (trans[y] >= 0 - ventana && trans[y] <= 0 + ventana)){
                Impaktado.jpg
            }

            */


            glLoadMatrixf(trans);
            //drawCube(80.0f, 0.0f, 0.0f, 40.0f);

            /*

            file2 = fopen("/sdcard/Matrices.txt", "a");

            sprintf(cadena,"Mat Trans \n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n", trans[0], trans[4], trans[8], trans[12], 
                                                                                                trans[1], trans[5], trans[9], trans[13],
                                                                                                trans[2], trans[6], trans[10], trans[14],
                                                                                                trans[3], trans[7], trans[11], trans[15]);
            if (file2 != NULL){
                fputs(cadena, file2);
                fflush(file2);
                fclose(file2);
            }
            */

            
            switch(in){
                case 0: dibujaDiana(150.0f, 0.0f, 0.0f, 0.0f, 255, 255, 255); break;
                case 1: dibujaDiana(150.0f, 0.0f, 0.0f, 0.0f, 255, 0, 0); break;
                case 2: dibujaDiana(150.0f, 0.0f, 0.0f, 0.0f, 0, 0, 255); break;
                case 3: dibujaDiana(150.0f, 0.0f, 0.0f, 0.0f, 0, 255, 255); break;
                case 4: dibujaDiana(150.0f, 0.0f, 0.0f, 0.0f, 0, 255, 0); break;
                case 5: dibujaDiana(150.0f, 0.0f, 0.0f, 0.0f, 255, 255, 0); break;
                case 6: dibujaDiana(150.0f, 0.0f, 0.0f, 0.0f, 255, 0, 255); break;
                case 7: dibujaDiana(150.0f, 0.0f, 0.0f, 0.0f, 30, 30, 30); break;
            } 


            clock_gettime(CLOCK_REALTIME, &res);
            end = 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;

            delta = end - start;
            file = fopen("/sdcard/FPS.txt", "a");

            sprintf(tiempo,"%f ms\n", delta);

            if (file != NULL){
                fputs(tiempo, file);
                fflush(file);
                fclose(file);
            }

            tiempoAnterior[in] = end;

        }
        else{


            detectado[in] = -1;

                transX[in] = -1;
                transY[in] = -1;
                transZ[in] = -1;



            /*
            if(tiempoTolerancia[in] <= 0){
                detectado[in] = -1;

                transX[in] = -1;
                transY[in] = -1;
                transZ[in] = -1;
            }
            else{

                clock_gettime(CLOCK_REALTIME, &res);
                end = 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;

                tiempoTolerancia[in] -= end - tiempoAnterior[in];

                //Solo falta terminar la parte de la tolerancia  

            }
            */
        }

        /*
        mtxLoadIdentityf(viewProjection);
        width = (float)viewPort[viewPortIndexWidth];
        height = (float)viewPort[viewPortIndexHeight];
        mtxOrthof(viewProjection, 0.0f, width, 0.0f, height, -1.0f, 1.0f);
        glStateCacheDisableDepthTest();
        */



    }
	
}
