;;
;; Local variables for use with Emacs
;; as an IDE for the ToolChain project 
;;
;;
;;
;; Fernando Carmona Varo 2012
;;

(
 (nil .
      (
       (gdb-use-separate-io-buffer . nil)
       (fill-column . 80)
       (outline-regexp  . ".*{")
       ))
 (c-mode .
	 ((eval .
	     (and
	      (setq dir-local-dir (locate-dominating-file (buffer-file-name) ".dir-locals.el"))
	      (setq compile-command (concat "cd " dir-local-dir " && ./compile.sh"))
	      (setq gud-gdb-command-name (concat dir-local-dir "_/debug.sh -i=mi"))
	      (setq grep-command (concat dir-local-dir "_/findInSource.sh . "))
	      (setq tags-table-list (list (concat dir-local-dir "TAGS")))
	      (setq semanticdb-project-roots (list "~/Source/ToolChain_STM32/CommonLibraries" "~/Source/ToolChain_STM32/ToolChain/**/*" dir-local-dir))
	      (ede-cpp-root-project "ttc_project" :file (concat dir-local-dir "required_version"))
	      ))
	  (c-file-style . "java")
	  ))
 )
