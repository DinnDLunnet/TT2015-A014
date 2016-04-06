package org.artoolkit.ar.samples.ARSimpleNative;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class OtraPartidaActivity extends Activity {
	private String nombre, nomPartida, ipServidor;

    public final static String EXTRA_NOMBRE_USUARIO = "com.gotcha.a014.interfazgotcha.NOMBRE_USUARIO";
    public final static String EXTRA_NOMBRE_PARTIDA = "com.gotcha.a014.interfazgotcha.NOMBRE_PARTIDA";
    public final static String EXTRA_IP_SERVIDOR = "com.gotcha.a014.interfazgotcha.IP_SERVIDOR";
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.otra_partida);
		
		Intent intent = getIntent();
        nombre = intent.getStringExtra(CrearPartidaActivity.EXTRA_NOMBRE_USUARIO);
        nomPartida = intent.getStringExtra(CrearPartidaActivity.EXTRA_NOMBRE_PARTIDA);
        ipServidor = intent.getStringExtra(CrearPartidaActivity.EXTRA_IP_SERVIDOR);
	}

	public void irVestibulo(View view) {
        Intent intent = new Intent(this, VestibuloActivity.class);
        intent.putExtra(EXTRA_NOMBRE_USUARIO, nombre);
        intent.putExtra(EXTRA_NOMBRE_PARTIDA, nomPartida);
        intent.putExtra(EXTRA_IP_SERVIDOR, ipServidor);

        /*
            Conectar con el servidor
        */

        startActivity(intent);
    }
	
	public void irModoJuego(View view) {
        Intent intent = new Intent(this, ModoJuegoActivity.class);
        intent.putExtra(EXTRA_NOMBRE_USUARIO, nombre);
        intent.putExtra(EXTRA_NOMBRE_PARTIDA, nomPartida);
        intent.putExtra(EXTRA_IP_SERVIDOR, ipServidor);

        /*
            Conectar con el servidor
        */

        startActivity(intent);
    }
}
