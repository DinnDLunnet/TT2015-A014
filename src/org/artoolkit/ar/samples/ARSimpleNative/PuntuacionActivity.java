package org.artoolkit.ar.samples.ARSimpleNative;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;

public class PuntuacionActivity extends Activity {
	private String nombre, nomPartida, ipServidor;

    public final static String EXTRA_NOMBRE_USUARIO = "com.gotcha.a014.interfazgotcha.NOMBRE_USUARIO";
    public final static String EXTRA_NOMBRE_PARTIDA = "com.gotcha.a014.interfazgotcha.NOMBRE_PARTIDA";
    public final static String EXTRA_IP_SERVIDOR = "com.gotcha.a014.interfazgotcha.IP_SERVIDOR";

    ArrayAdapter<String> adapter;

    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.puntuacion);
		
		Intent intent = getIntent();
        nombre = intent.getStringExtra(CrearPartidaActivity.EXTRA_NOMBRE_USUARIO);
        nomPartida = intent.getStringExtra(CrearPartidaActivity.EXTRA_NOMBRE_PARTIDA);
        ipServidor = intent.getStringExtra(CrearPartidaActivity.EXTRA_IP_SERVIDOR);

        ListView list = (ListView) findViewById(R.id.listView);
        ArrayList<String> arrayList = new ArrayList<String>();

        adapter = new ArrayAdapter<String>(getApplicationContext(), R.layout.mi_lista, arrayList);
        list.setAdapter(adapter);

        arrayList.add(nombre);
        // next thing you have to do is check if your adapter has changed
        adapter.notifyDataSetChanged();
	}

	public void otraPartida(View view) {
        Intent intent = new Intent(this, OtraPartidaActivity.class);
        intent.putExtra(EXTRA_NOMBRE_USUARIO, nombre);
        intent.putExtra(EXTRA_NOMBRE_PARTIDA, nomPartida);
        intent.putExtra(EXTRA_IP_SERVIDOR, ipServidor);

        /*
            Conectar con el servidor
        */

        startActivity(intent);
    }
}
