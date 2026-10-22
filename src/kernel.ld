ENTRY(booting)

SECTIONS {
    . = 0x7C00;

    .boot : { *(.boot) }

    . = 0x7E00;

    .kernel : {
        *(.text);
        *(.rodata);
        *(.rodata.*);
        *(.data);
        *(.bss);
    }

    .=ALIGN(8)
    kernel_end = .;


    /DISCARD/ : {
        *(.eh_frame)
        *(.note.*)
        *(.comment)
    }
}