from flask import Flask, jsonify, request
from datetime import datetime 

app = Flask(__name__)
historico_dados = []


@app.route("/api/rfid", methods=["POST"])
def receber_rfid():
    global historico_dados
    try:
        dados_recebidos = request.get_json()

        hora_da_maquina = datetime.now().strftime("%d/%m/%Y %H:%M:%S")

        dados_recebidos["timestamp"] = hora_da_maquina

        print(f"Dado registrado com a hora do PC: {dados_recebidos}")

        historico_dados.append(dados_recebidos)

        if len(historico_dados) > 100:
            historico_dados.pop(0)

        return jsonify({"status": "sucesso", "mensagem": "Dados armazenados"}), 200
    except Exception as e:
        print(f"Erro: {e}")
        return jsonify({"status": "erro", "mensagem": str(e)}), 400


@app.get("/taps")
def index():
    return jsonify(historico_dados)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5432, debug=False)