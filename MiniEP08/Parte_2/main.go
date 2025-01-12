package main

import (
	"encoding/json"
	"log"
	"os"
	"sync"

	maelstrom "github.com/jepsen-io/maelstrom/demo/go"
)

type Recurso struct {
	counter int
	mux     sync.Mutex
}

func main() {
	n := maelstrom.NewNode()

	recurso := &Recurso{
		counter: 0,
	}

	// Register a handler for the "generate" message that responds with an "generate_ok" and a unique ID.
	n.Handle("generate", func(msg maelstrom.Message) error {
		// Unmarshal the message body as an loosely-typed map.
		var body map[string]any
		if err := json.Unmarshal(msg.Body, &body); err != nil {
			return err
		}

		// Get current counter value and increment it.
		body["id"] = getAndIncrementCounter(recurso) // Operação atômica

		// Update the message type.
		body["type"] = "generate_ok"

		return n.Reply(msg, body)
	})

	// Execute the node's message loop. This will run until STDIN is closed.
	if err := n.Run(); err != nil {
		log.Printf("ERROR: %s", err)
		os.Exit(1)
	}
}

func getAndIncrementCounter(recurso *Recurso) int {
	recurso.mux.Lock()
	defer recurso.mux.Unlock() // Destrava o Mutex, após o retorno
	
	id := recurso.counter
	recurso.counter++

	return id
}
