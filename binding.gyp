{
	'targets': [
		{
			'target_name': 'jsdx_display',
			'sources': [
				'src/jsdx_display.cpp',
				'src/x11.cpp'
			],
			'conditions': [
				['OS=="linux"', {
					'cflags': [
						'<!@(pkg-config --cflags x11 xrandr)'
					],
					'ldflags': [
						'<!@(pkg-config  --libs-only-L --libs-only-other x11 xrandr)'
					],
					'libraries': [
						'<!@(pkg-config  --libs-only-l --libs-only-other x11 xrandr)'
					],
					'defines': [
						'USE_X11'
					]
				}]
			]
		}
	]
}
