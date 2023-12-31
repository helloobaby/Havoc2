package cmd

import (
	"os"
	"os/exec"

	"github.com/spf13/cobra"
)

var CobraClient = &cobra.Command{
	Use:          "client",
	Short:        "client command",
	SilenceUsage: true,
	RunE: func(cmd *cobra.Command, args []string) error {

		client := exec.Command("client/Havoc", args...)
		client.Stdout = os.Stdout
		client.Stderr = os.Stderr

		if err := client.Run(); err != nil {
			return err
		}

		return nil
	},
}
