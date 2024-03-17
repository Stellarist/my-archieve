package com.example.nutrition.entity;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

import com.fasterxml.jackson.databind.ObjectMapper;

public class PyScript {
    public static String quote(String uri, Object object) {
        try {
            String json=new ObjectMapper().writeValueAsString(object);
            HttpClient httpClient = HttpClient.newHttpClient();
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(new URI(uri))
                    .header("Content-Type", "application/json")
                    .POST(HttpRequest.BodyPublishers.ofString(json))
                    .build();

            HttpResponse<String> response = httpClient.send(request, HttpResponse.BodyHandlers.ofString());
            String pythonOutput = response.body();

            return pythonOutput;

        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
