var express = require('express'),
    app = express.createServer(express.logger()),
    port = process.env.PORT || 5000,
    SECRET_KEY = 'bacon';

var state = {
    serverStarted: Date.now(),

    forst: {
        on: false,
        lastPing: null,
        pingCount: 0
    },

    sekund: {
        on: false,
        lastPing: null,
        pingCount: 0
    }
};

app.get('/', function(req, res) {
    res.send('ljus');
});

app.get('/status', function(req, res) {
    res.send(state);
});

app.get('/ping', function(req, res) {
    if (req.query.key != SECRET_KEY) {
        res.send('BAD KEY');
    }

    if (req.query.on == null) {
        res.send('MISSING ON VALUE');
    }

    if (req.query.id == null) {
        res.send('MISSING ID');
    }

    state[req.query.id] = {
        on: (req.query.on == '1') ? true : false,
        lastPing: Date.now(),
        pingCount: state[req.query.id].pingCount + 1
    };

    if (req.query.id == 'forst') {
        res.send("{" + state.sekund.on + "}");
    } else {
        res.send("{" + state.forst.on + "}");
    }
});

app.listen(port, function() {
    console.log("Listening on " + port);
});