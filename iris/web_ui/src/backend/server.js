const express = require('express');
const cors = require('cors');
const lcm = require('lcm-node'); // certifique-se de ter as mensagens .lcm compiladas para JS
const app = express();
const port = 4000;

app.use(cors());

let latestData = {
  timestamp: 0,
  robots_size: 0,
  team_blue: false,
  score_yellow: 0,
  score_blue: 0,
  field_length: 0,
  processo: '',
  estrategia: '',
  ssl_vision: false,
  team_blue_status: false,
};

// Inicializar LCM
const lc = new lcm.LCM();

let msg_GC, msg_vision, msg_tartarus, msg_ia;

// Substitua `game_controller`, `vision`, `tartarus`, `ia` pelos seus módulos LCM compilados para JS
const game_controller = require('../../../../data_lcm/data/game_controller'); 
const vision = require('../../../../data_lcm/data/vision');
const tartarus = require('../../../../data_lcm/data/tartarus');
const ia = require('../../../../data_lcm/data/ia');

lc.subscribe('GC', (channel, buffer) => {
  msg_GC = game_controller.decode(buffer);
});

lc.subscribe('vision', (channel, buffer) => {
  msg_vision = vision.decode(buffer);
});

lc.subscribe('tartarus', (channel, buffer) => {
  msg_tartarus = tartarus.decode(buffer);

  if (msg_GC) {
    if (msg_GC.blue.name === 'Cerberus') {
      msg_tartarus.team_blue = true;
    } else if (msg_GC.yellow.name === 'Cerberus') {
      msg_tartarus.team_blue = false;
    } else {
      msg_tartarus.team_blue = true;
    }
  }
});

lc.subscribe('IA', (channel, buffer) => {
  msg_ia = ia.decode(buffer);

  // Monta os dados reais com base nas últimas mensagens recebidas
  if (msg_GC && msg_vision && msg_tartarus) {
    latestData = {
      timestamp: msg_vision.timestamp,
      robots_size: msg_ia.robots_size,
      team_blue: msg_GC.team_blue,
      score_yellow: msg_GC.yellow.score,
      score_blue: msg_GC.blue.score,
      field_length: msg_vision.field.field_length,
      processo: msg_ia.processo,
      estrategia: msg_ia.estrategia,
      ssl_vision: msg_tartarus.ssl_vision,
      team_blue_status: msg_tartarus.team_blue,
    };
  }
});

// Endpoint que serve os dados atuais
app.get('/data', (req, res) => {
  res.json(latestData);
});

app.listen(port, () => {
  console.log(`Servidor rodando na porta ${port}`);
});
