package main

import (
	"encoding/json"
	"log"
	"os"
	"fmt"
	"sync"
	"time"

	maelstrom "github.com/jepsen-io/maelstrom/demo/go"
)

type Recurso struct {
	nodeID  string
	counter int
	mux     sync.Mutex
}

func main() {
	n := maelstrom.NewNode()

	recurso := &Recurso{
		nodeID:  n.ID(),
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

func getAndIncrementCounter(recurso *Recurso) string {
	recurso.mux.Lock()
	defer recurso.mux.Unlock() // Destrava o Mutex, após o retorno

	timestamp := time.Now().UnixNano()

	//id := recurso.counter
	id := fmt.Sprintf("%s-%d-%d", recurso.nodeID, recurso.counter, timestamp)
	recurso.counter++

	return id
}
