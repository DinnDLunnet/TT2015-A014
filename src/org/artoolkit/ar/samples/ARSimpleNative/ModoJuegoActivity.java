package org.artoolkit.ar.samples.ARSimpleNative;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.widget.TextView;

public class ModoJuegoActivity extends Activity {
    private String nombre, ipServidor;

    public final static String EXTRA_NOMBRE_USUARIO = "com.gotcha.a014.interfazgotcha.NOMBRE_USUARIO";
    public final static String EXTRA_IP_SERVIDOR = "com.gotcha.a014.interfazgotcha.IP_SERVIDOR";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.modo_juego);

        Intent intent = getIntent();
        nombre = intent.getStringExtra(GUIActivity.EXTRA_NOMBRE_USUARIO);
        ipServidor = intent.getStringExtra(GUIActivity.EXTRA_IP_SERVIDOR);

        TextView textView = (TextView) findViewById(R.id.textView3);
        textView.setText(nombre);

    }

    public void irCrearPartida(View view) {
        Intent intent = new Intent(this, CrearPartidaActivity.class);
        intent.putExtra(EXTRA_NOMBRE_USUARIO, nombre);
        intent.putExtra(EXTRA_IP_SERVIDOR, ipServidor);
        startActivity(intent);
    }

    public void irUnirsePartida(View view) {
        Intent intent = new Intent(this, UnirsePartidaActivity.class);
        intent.putExtra(EXTRA_NOMBRE_USUARIO, nombre);
        intent.putExtra(EXTRA_IP_SERVIDOR, ipServidor);
        startActivity(intent);
    }

}
