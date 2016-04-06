/*
 *  ARSimpleNative.java
 *  ARToolKit5
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
 *  Copyright 2015 Daqri, LLC.
 *  Copyright 2011-2015 ARToolworks, Inc.
 *
 *  Author(s): Julian Looser, Philip Lamb
 *
 */

package org.artoolkit.ar.samples.ARSimpleNative;

import org.artoolkit.ar.base.ARActivity;
import org.artoolkit.ar.base.rendering.ARRenderer;
import org.artoolkit.ar.samples.ARSimpleNative.R;

import android.content.Intent;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.util.Log;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.view.*;
import android.view.View.OnTouchListener;
import android.view.ViewGroup.LayoutParams;



public class ARSimpleNative extends ARActivity implements OnTouchListener{
	
	public final static String EXTRA_NOMBRE_USUARIO = "com.gotcha.a014.interfazgotcha.NOMBRE_USUARIO";
    public final static String EXTRA_NOMBRE_PARTIDA = "com.gotcha.a014.interfazgotcha.NOMBRE_PARTIDA";
    public final static String EXTRA_IP_SERVIDOR = "com.gotcha.a014.interfazgotcha.IP_SERVIDOR";
    
    private String nombre, nomPartida, ipServidor;

	private SimpleNativeRenderer simpleNativeRenderer = new SimpleNativeRenderer();
	private FrameLayout mFrame;
	private TextView emergente;
	private int idImpacto;
	private int soundId;
	private SoundPool sp;
	
	private long startTime = 3000000L;
	private long realStart;
	private Handler customHandler = new Handler();
	long timeInMilliseconds = 0L;
	long timeSwapBuff = 0L;
	long updatedTime = 0L;
	private Runnable updateTimerThread;
	
	private boolean calentado;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(
        WindowManager.LayoutParams.FLAG_FULLSCREEN,
        WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        Intent intent = getIntent();
        nombre = intent.getStringExtra(CrearPartidaActivity.EXTRA_NOMBRE_USUARIO);
        nomPartida = intent.getStringExtra(CrearPartidaActivity.EXTRA_NOMBRE_PARTIDA);
        ipServidor = intent.getStringExtra(CrearPartidaActivity.EXTRA_IP_SERVIDOR);
        
        setContentView(R.layout.main);        
        mFrame = (FrameLayout) findViewById(R.id.mainLayout);
        mFrame.setOnTouchListener(this);

        emergente = new TextView(this);
        emergente.setText("X");
        emergente.setId(5);
        emergente.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT,LayoutParams.WRAP_CONTENT));

        sp = new SoundPool(5, AudioManager.STREAM_MUSIC, 0);
        soundId = sp.load(this, R.raw.laser, 1);
        realStart = SystemClock.uptimeMillis();
        updatedTime = startTime;
        
        updateTimerThread = new Runnable() {
        	public void run() {
        		timeInMilliseconds = SystemClock.uptimeMillis() - realStart;
        		updatedTime -= timeInMilliseconds;
        		int secs = (int) (updatedTime / 1000);
        		int mins = secs / 60;
        		secs = secs % 60;
        		int milliseconds = (int) (updatedTime % 1000);
        		
        		if(updatedTime <= 0){
        			customHandler.removeCallbacks(updateTimerThread);
        			irPuntuaciones(findViewById(android.R.id.content));
        		}
        		else{
        			customHandler.postDelayed(this, 1000);
        		}
		
        	}
        };
        
        
        
        customHandler.postDelayed(updateTimerThread, 0);
        
        calentado = false;
    }
    
    public void onStop() {
    	SimpleNativeRenderer.demoShutdown();
    	sp.release();
    	super.onStop();
    }

    @Override
    protected ARRenderer supplyRenderer() {
    	return simpleNativeRenderer;
    }
    
    @Override
    protected FrameLayout supplyFrameLayout() {
    	return (FrameLayout)this.findViewById(R.id.mainLayout);
    	
    }
    
    public void irPuntuaciones(View view) {
        Intent intent = new Intent(this, PuntuacionActivity.class);
        intent.putExtra(EXTRA_NOMBRE_USUARIO, nombre);
        intent.putExtra(EXTRA_NOMBRE_PARTIDA, nomPartida);
        intent.putExtra(EXTRA_IP_SERVIDOR, ipServidor);

        /*
            Conectar con el servidor
        */

        startActivity(intent);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        // TODO Auto-generated method stub
    	if(mFrame == v && calentado == false) {
    		Log.i(TAG, "onTouchEvent");
            
    		sp.play(soundId, 1, 1, 0, 0, 1);
    		
    		calentado = true;
    		
    		Handler handler = new Handler();
            handler.postDelayed(new Runnable() {
               @Override
               public void run() {
                   calentado = false; 
               }
            }, 500);

            idImpacto = simpleNativeRenderer.habreImpactado();
            if(idImpacto >= 0){
            	
            	final Toast toast = Toast.makeText(this, "Jugador "+idImpacto+" impactado!", Toast.LENGTH_SHORT);
                toast.show();

                handler = new Handler();
                    handler.postDelayed(new Runnable() {
                       @Override
                       public void run() {
                           toast.cancel(); 
                    }
                }, 500);

            }          
            
    		return true;
    	}
    	
        
        return false;
    }


}



