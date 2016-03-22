package org.artoolkit.ar.samples.ARSimpleNative;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.widget.EditText;

public class CrearPartidaActivity extends Activity {
    private String nombre, ipServidor;

    public final static String EXTRA_NOMBRE_USUARIO = "com.gotcha.a014.interfazgotcha.NOMBRE_USUARIO";
    public final static String EXTRA_NOMBRE_PARTIDA = "com.gotcha.a014.interfazgotcha.NOMBRE_PARTIDA";
    public final static String EXTRA_IP_SERVIDOR = "com.gotcha.a014.interfazgotcha.IP_SERVIDOR";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.crear_partida);

        Intent intent = getIntent();
        nombre = intent.getStringExtra(ModoJuegoActivity.EXTRA_NOMBRE_USUARIO);
        ipServidor = intent.getStringExtra(ModoJuegoActivity.EXTRA_IP_SERVIDOR);
    }

    public void paramCrearPartida(View view) {
        Intent intent = new Intent(this, VestibuloActivity.class);
        intent.putExtra(EXTRA_NOMBRE_USUARIO, nombre);
        EditText editText = (EditText) findViewById(R.id.editText2);
        String message = editText.getText().toString();
        intent.putExtra(EXTRA_NOMBRE_PARTIDA, message);
        intent.putExtra(EXTRA_IP_SERVIDOR, ipServidor);

        /*
            Conectar con el servidor
        */

        startActivity(intent);
    }

}
