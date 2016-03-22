package org.artoolkit.ar.samples.ARSimpleNative;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import java.util.ArrayList;

public class UnirsePartidaActivity extends Activity {

    public final static String EXTRA_NOMBRE_USUARIO = "com.gotcha.a014.interfazgotcha.NOMBRE_USUARIO";
    public final static String EXTRA_NOMBRE_PARTIDA = "com.gotcha.a014.interfazgotcha.NOMBRE_PARTIDA";
    public final static String EXTRA_IP_SERVIDOR = "com.gotcha.a014.interfazgotcha.IP_SERVIDOR";
    private String nombre, nomPartida, ipServidor;

    ArrayAdapter<String> adapter2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.unirse_partida);

        Intent intent = getIntent();
        nombre = intent.getStringExtra(ModoJuegoActivity.EXTRA_NOMBRE_USUARIO);
        ipServidor = intent.getStringExtra(ModoJuegoActivity.EXTRA_IP_SERVIDOR);


        ListView list2 = (ListView) findViewById(R.id.listView2);
        ArrayList<String> arrayList2 = new ArrayList<String>();

        adapter2 = new ArrayAdapter<String>(getApplicationContext(), R.layout.mi_lista, arrayList2);
        list2.setAdapter(adapter2);

        list2.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapter2, View v, int position, long arg3) {
                String value = (String)adapter2.getItemAtPosition(position);

                Intent intent = new Intent(adapter2.getContext(), VestibuloActivity.class);
                intent.putExtra(EXTRA_NOMBRE_USUARIO, nombre);
                intent.putExtra(EXTRA_NOMBRE_PARTIDA, value);
                intent.putExtra(EXTRA_IP_SERVIDOR, ipServidor);

                /*
                    Enviar mensaje al servidor
                */

                startActivity(intent);

            }
        });

        /*

        Conectar con el servidor y pedirle las partidas disponibles

         */

        arrayList2.add("Le Doge Fest");
        // next thing you have to do is check if your adapter has changed
        adapter2.notifyDataSetChanged();

    }



}
