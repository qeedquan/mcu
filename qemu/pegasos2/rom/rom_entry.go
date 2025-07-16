// relocate pegasos2.rom entry point

package main

import (
	"encoding/binary"
	"flag"
	"fmt"
	"log"
	"os"
)

var flags struct {
	entry int
}

func main() {
	log.SetFlags(0)
	log.SetPrefix("rom_entry: ")

	flag.IntVar(&flags.entry, "e", 0x4000, "entry point")
	flag.Usage = usage
	flag.Parse()
	if flag.NArg() != 2 {
		usage()
	}

	data, err := os.ReadFile(flag.Arg(0))
	check(err)

	if len(data) != 512*1024 {
		log.Fatal("invalid rom size")
	}

	relocate(data, 0, flags.entry)
	relocate(data, 0x108, flags.entry)

	err = os.WriteFile(flag.Arg(1), data, 0644)
	check(err)
}

func relocate(data []byte, off, entry int) {
	loc := uint16(entry - off)
	binary.BigEndian.PutUint16(data[off+2:], loc)
}

func check(err error) {
	if err != nil {
		log.Fatal(err)
	}
}

func usage() {
	fmt.Fprintln(os.Stderr, "usage: [options] <input> <output>")
	flag.PrintDefaults()
	os.Exit(2)
}
