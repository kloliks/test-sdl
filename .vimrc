set path+=include

map <F9> :make! --no-print-directory -C build<CR>
imap <F9> <ESC>:make! --no-print-directory -C build<CR>
vmap <F9> <ESC>:make! --no-print-directory -C build<CR>

map <F12> :!./build/src/test-sdl<CR>
