package org.artoolkit.ar.samples.ARSimpleNative;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.widget.EditText;

public class GUIActivity extends Activity {

	public final static String EXTRA_NOMBRE_USUARIO = "com.gotcha.a014.interfazgotcha.NOMBRE_USUARIO";
    public final static String EXTRA_IP_SERVIDOR = "com.gotcha.a014.interfazgotcha.IP_SERVIDOR";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.gui);

    }

    /** Called when the user clicks the Send button */
    public void sendMessage(View view) {
        Intent intent = new Intent(this, ModoJuegoActivity.class);
        EditText editText = (EditText) findViewById(R.id.editText);
        EditText editText3 = (EditText) findViewById(R.id.editText3);
        String nombre = editText.getText().toString();
        String ipServidor = editText3.getText().toString();
        intent.putExtra(EXTRA_NOMBRE_USUARIO, nombre);
        intent.putExtra(EXTRA_IP_SERVIDOR, ipServidor);
        startActivity(intent);
    	}
}
