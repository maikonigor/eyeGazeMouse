/*jslint */
/*global AdobeEdge: false, window: false, document: false, console:false, alert: false */
(function (compId) {

    "use strict";
    var im='images/',
        aud='media/',
        vid='media/',
        js='js/',
        fonts = {
        },
        opts = {
            'gAudioPreloadPreference': 'auto',
            'gVideoPreloadPreference': 'auto'
        },
        resources = [
        ],
        scripts = [
            js+"jquery-2.0.3.min.js"
        ],
        symbols = {
            "stage": {
                version: "6.0.0",
                minimumCompatibleVersion: "5.0.0",
                build: "6.0.0.400",
                scaleToFit: "both",
                centerStage: "both",
                resizeInstances: false,
                content: {
                    dom: [
                        {
                            id: 'Rectangle',
                            type: 'rect',
                            rect: ['0px', '0px', '472px', '20px', 'auto', 'auto'],
                            fill: ["rgba(104,165,255,1.00)"],
                            stroke: [1,"rgba(0,0,0,1)","solid"]
                        },
                        {
                            id: 'Rectangle3Copy',
                            type: 'rect',
                            rect: ['75px', '2px', '86px', '16px', 'auto', 'auto'],
                            fill: ["rgba(0,0,0,1)"],
                            stroke: [1,"rgb(0, 0, 0)","solid"]
                        },
                        {
                            id: 'Rectangle3',
                            type: 'rect',
                            rect: ['302px', '2px', '60px', '16px', 'auto', 'auto'],
                            fill: ["rgba(0,0,0,1)"],
                            stroke: [1,"rgb(0, 0, 0)","solid"]
                        },
                        {
                            id: 'RectangleCopy',
                            type: 'rect',
                            rect: ['474px', '0px', '73px', '20px', 'auto', 'auto'],
                            fill: ["rgba(104,165,255,1.00)"],
                            stroke: [1,"rgba(0,0,0,1)","solid"]
                        },
                        {
                            id: 'msn_saida',
                            type: 'text',
                            rect: ['76px', '3px', '87px', '17px', 'auto', 'auto'],
                            text: "<p style=\"margin: 0px;\">---------------</p>",
                            align: "center",
                            font: ['Arial, Helvetica, sans-serif', [14, "px"], "rgba(255,255,255,1.00)", "normal", "none", "", "break-word", "normal"]
                        },
                        {
                            id: 'relogio',
                            symbolName: 'relogio',
                            type: 'rect',
                            rect: ['404', '2', '68', '18', 'auto', 'auto']
                        },
                        {
                            id: 'msn_saidaCopy2',
                            type: 'text',
                            rect: ['302px', '3px', '62px', '17px', 'auto', 'auto'],
                            text: "<p style=\"margin: 0px;\">​00:00:00</p>",
                            align: "center",
                            font: ['Arial, Helvetica, sans-serif', [14, "px"], "rgba(255,255,255,1.00)", "normal", "none", "", "break-word", "normal"]
                        },
                        {
                            id: 'bt_1',
                            symbolName: 'bt_1',
                            type: 'rect',
                            rect: ['108', '88', '37', '37', 'auto', 'auto'],
                            cursor: 'pointer'
                        },
                        {
                            id: 'bt_2',
                            symbolName: 'bt_2',
                            type: 'rect',
                            rect: ['145', '88', '36', '37', 'auto', 'auto'],
                            cursor: 'pointer'
                        },
                        {
                            id: 'bt_4',
                            symbolName: 'bt_4',
                            type: 'rect',
                            rect: ['0', '21px', '108', '104', 'auto', 'auto'],
                            cursor: 'pointer'
                        },
                        {
                            id: 'bt_5',
                            symbolName: 'bt_5',
                            type: 'rect',
                            rect: ['0px', '125px', '181', '182', 'auto', 'auto'],
                            cursor: 'pointer'
                        },
                        {
                            id: 'bt_6',
                            symbolName: 'bt_6',
                            type: 'rect',
                            rect: ['181', '21', '293', '285', 'auto', 'auto'],
                            cursor: 'pointer'
                        },
                        {
                            id: 'Rectangle4',
                            type: 'rect',
                            rect: ['474px', '22px', '73px', '282px', 'auto', 'auto'],
                            fill: ["rgba(57,57,57,1.00)"],
                            stroke: [1,"rgb(0, 0, 0)","solid"]
                        },
                        {
                            id: 'Text3',
                            type: 'text',
                            rect: ['477px', '4px', '69px', '15px', 'auto', 'auto'],
                            text: "<p style=\"margin: 0px;\"><span style=\"font-size: 14px;\">LOG</span></p>",
                            align: "center",
                            font: ['Arial, Helvetica, sans-serif', [8, "px"], "rgba(255,255,255,1.00)", "400", "none", "normal", "break-word", "normal"],
                            textStyle: ["", "", "", "", "none"]
                        },
                        {
                            id: 'log_eventos',
                            type: 'text',
                            rect: ['477px', '24px', '73px', '282px', 'auto', 'auto'],
                            overflow: 'auto',
                            text: "<p style=\"margin: 0px;\">​<span style=\"color: rgb(238, 238, 238);\">​</span></p>",
                            align: "center",
                            font: ['Arial, Helvetica, sans-serif', [8, "px"], "rgba(238,238,238,1.00)", "400", "none", "normal", "break-word", "normal"],
                            textStyle: ["", "", "", "", "none"]
                        },
                        {
                            id: 'bt_3',
                            symbolName: 'bt_3',
                            type: 'rect',
                            rect: ['108', '21', '73', '67', 'auto', 'auto'],
                            cursor: 'pointer'
                        },
                        {
                            id: 'Text',
                            type: 'text',
                            rect: ['267px', '4px', '36px', '15px', 'auto', 'auto'],
                            text: "<p style=\"margin: 0px;\">Start:</p>",
                            align: "left",
                            font: ['Arial, Helvetica, sans-serif', [14, "px"], "rgba(255,255,255,1)", "400", "none", "normal", "break-word", "normal"],
                            textStyle: ["", "", "", "", "none"]
                        },
                        {
                            id: 'TextCopy2',
                            type: 'text',
                            rect: ['3px', '4px', '75px', '15px', 'auto', 'auto'],
                            text: "<p style=\"margin: 0px;\">Last Event:</p>",
                            align: "left",
                            font: ['Arial, Helvetica, sans-serif', [14, "px"], "rgba(255,255,255,1)", "400", "none", "normal", "break-word", "normal"],
                            textStyle: ["", "", "", "", "none"]
                        },
                        {
                            id: 'Rectangle3Copy2',
                            type: 'rect',
                            rect: ['226px', '2px', '37px', '16px', 'auto', 'auto'],
                            fill: ["rgba(0,0,0,1)"],
                            stroke: [1,"rgb(0, 0, 0)","solid"]
                        },
                        {
                            id: 'TextCopy3',
                            type: 'text',
                            rect: ['166px', '4px', '62px', '15px', 'auto', 'auto'],
                            text: "<p style=\"margin: 0px;\">Attempts:</p>",
                            align: "left",
                            font: ['Arial, Helvetica, sans-serif', [14, "px"], "rgba(255,255,255,1)", "400", "none", "normal", "break-word", "normal"],
                            textStyle: ["", "", "", "", "none"]
                        },
                        {
                            id: 'TextCopy',
                            type: 'text',
                            rect: ['367px', '4px', '36px', '15px', 'auto', 'auto'],
                            text: "<p style=\"margin: 0px;\">Time:</p>",
                            align: "left",
                            font: ['Arial, Helvetica, sans-serif', [14, "px"], "rgba(255,255,255,1)", "400", "none", "normal", "break-word", "normal"],
                            textStyle: ["", "", "", "", "none"]
                        },
                        {
                            id: 'erros',
                            type: 'text',
                            rect: ['227px', '4px', '37px', '15px', 'auto', 'auto'],
                            text: "<p style=\"margin: 0px;\">​0</p>",
                            align: "center",
                            font: ['Arial, Helvetica, sans-serif', [14, "px"], "rgba(255,255,255,1)", "400", "none", "normal", "break-word", "normal"],
                            textStyle: ["", "", "", "", "none"]
                        }
                    ],
                    style: {
                        '${Stage}': {
                            isStage: true,
                            rect: ['null', 'null', '549px', '306px', 'auto', 'auto'],
                            overflow: 'hidden',
                            fill: ["rgba(255,255,255,1)"]
                        }
                    }
                },
                timeline: {
                    duration: 2043,
                    autoPlay: true,
                    data: [

                    ]
                }
            },
            "bt_1": {
                version: "6.0.0",
                minimumCompatibleVersion: "5.0.0",
                build: "6.0.0.400",
                scaleToFit: "none",
                centerStage: "none",
                resizeInstances: false,
                content: {
                    dom: [
                        {
                            rect: ['0px', '0px', '35px', '35px', 'auto', 'auto'],
                            stroke: [1, 'rgb(0, 0, 0)', 'solid'],
                            id: 'Rectangle3Copy',
                            opacity: '1',
                            type: 'rect',
                            fill: ['rgba(200,200,200,1.00)', [270, [['rgba(164,164,164,1.00)', 0], ['rgba(255,255,255,1.00)', 100]]]]
                        },
                        {
                            rect: ['9px', '5px', '19px', '27px', 'auto', 'auto'],
                            textStyle: ['', '', '', '', 'none'],
                            font: ['Arial, Helvetica, sans-serif', [25, 'px'], 'rgba(0,0,0,1)', '400', 'none', 'normal', 'break-word', 'normal'],
                            id: 'Text2',
                            text: '<p style=\"margin: 0px;\">​1</p>',
                            align: 'center',
                            type: 'text'
                        }
                    ],
                    style: {
                        '${symbolSelector}': {
                            isStage: 'true',
                            rect: [undefined, undefined, '37px', '37px']
                        }
                    }
                },
                timeline: {
                    duration: 2043,
                    autoPlay: false,
                    labels: {
                        "inicio": 0,
                        "brilhando": 1000,
                        "fim": 2000
                    },
                    data: [
                        [
                            "eid26",
                            "background-image",
                            1000,
                            500,
                            "linear",
                            "${Rectangle3Copy}",
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]],
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]]
                        ],
                        [
                            "eid27",
                            "background-image",
                            1500,
                            500,
                            "linear",
                            "${Rectangle3Copy}",
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]],
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]]
                        ]
                    ]
                }
            },
            "bt_2": {
                version: "6.0.0",
                minimumCompatibleVersion: "5.0.0",
                build: "6.0.0.400",
                scaleToFit: "none",
                centerStage: "none",
                resizeInstances: false,
                content: {
                    dom: [
                        {
                            rect: ['0px', '0px', '34px', '35px', 'auto', 'auto'],
                            stroke: [1, 'rgb(0, 0, 0)', 'solid'],
                            id: 'Rectangle3Copy2',
                            opacity: '1',
                            type: 'rect',
                            fill: ['rgba(200,200,200,1.00)', [270, [['rgba(164,164,164,1.00)', 0], ['rgba(255,255,255,1.00)', 100]]]]
                        },
                        {
                            rect: ['8px', '5px', '19px', '27px', 'auto', 'auto'],
                            textStyle: ['', '', '', '', 'none'],
                            font: ['Arial, Helvetica, sans-serif', [25, 'px'], 'rgba(0,0,0,1)', '400', 'none', 'normal', 'break-word', 'normal'],
                            id: 'Text2Copy',
                            text: '<p style=\"margin: 0px;\">​2</p>',
                            align: 'center',
                            type: 'text'
                        }
                    ],
                    style: {
                        '${symbolSelector}': {
                            isStage: 'true',
                            rect: [undefined, undefined, '36px', '37px']
                        }
                    }
                },
                timeline: {
                    duration: 2043,
                    autoPlay: false,
                    labels: {
                        "inicio": 0,
                        "brilhando": 1000,
                        "fim": 2000
                    },
                    data: [
                        [
                            "eid24",
                            "background-image",
                            1000,
                            500,
                            "linear",
                            "${Rectangle3Copy2}",
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]],
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]]
                        ],
                        [
                            "eid25",
                            "background-image",
                            1500,
                            500,
                            "linear",
                            "${Rectangle3Copy2}",
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]],
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]]
                        ]
                    ]
                }
            },
            "bt_3": {
                version: "6.0.0",
                minimumCompatibleVersion: "5.0.0",
                build: "6.0.0.400",
                scaleToFit: "none",
                centerStage: "none",
                resizeInstances: false,
                content: {
                    dom: [
                        {
                            rect: ['0px', '0px', '71px', '65px', 'auto', 'auto'],
                            stroke: [1, 'rgb(0, 0, 0)', 'solid'],
                            id: 'Rectangle3',
                            opacity: '1',
                            type: 'rect',
                            fill: ['rgba(200,200,200,1.00)', [43, [['rgba(164,164,164,1.00)', 0], ['rgba(255,255,255,1.00)', 100]]]]
                        },
                        {
                            rect: ['27px', '20px', '19px', '27px', 'auto', 'auto'],
                            textStyle: ['', '', '', '', 'none'],
                            font: ['Arial, Helvetica, sans-serif', [25, 'px'], 'rgba(0,0,0,1)', '400', 'none', 'normal', 'break-word', 'normal'],
                            id: 'Text2Copy2',
                            text: '<p style=\"margin: 0px;\">​3</p>',
                            align: 'center',
                            type: 'text'
                        }
                    ],
                    style: {
                        '${symbolSelector}': {
                            isStage: 'true',
                            rect: [undefined, undefined, '73px', '67px']
                        }
                    }
                },
                timeline: {
                    duration: 2043,
                    autoPlay: false,
                    labels: {
                        "inicio": 0,
                        "brilhando": 1000,
                        "fim": 2000
                    },
                    data: [
                        [
                            "eid22",
                            "background-image",
                            1000,
                            500,
                            "linear",
                            "${Rectangle3}",
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]],
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]]
                        ],
                        [
                            "eid23",
                            "background-image",
                            1500,
                            500,
                            "linear",
                            "${Rectangle3}",
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]],
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]]
                        ]
                    ]
                }
            },
            "bt_4": {
                version: "6.0.0",
                minimumCompatibleVersion: "5.0.0",
                build: "6.0.0.400",
                scaleToFit: "none",
                centerStage: "none",
                resizeInstances: false,
                content: {
                    dom: [
                        {
                            rect: ['0px', '0px', '106px', '102px', 'auto', 'auto'],
                            stroke: [1, 'rgb(0, 0, 0)', 'solid'],
                            id: 'Rectangle2',
                            opacity: '1',
                            type: 'rect',
                            fill: ['rgba(200,200,200,1.00)', [43, [['rgba(164,164,164,1.00)', 6], ['rgba(255,255,255,1.00)', 100]]]]
                        },
                        {
                            rect: ['44px', '38px', '19px', '27px', 'auto', 'auto'],
                            textStyle: ['', '', '', '', 'none'],
                            font: ['Arial, Helvetica, sans-serif', [25, 'px'], 'rgba(0,0,0,1)', '400', 'none', 'normal', 'break-word', 'normal'],
                            id: 'Text2Copy3',
                            text: '<p style=\"margin: 0px;\">​4</p>',
                            align: 'center',
                            type: 'text'
                        }
                    ],
                    style: {
                        '${symbolSelector}': {
                            isStage: 'true',
                            rect: [undefined, undefined, '108px', '104px']
                        }
                    }
                },
                timeline: {
                    duration: 2043,
                    autoPlay: false,
                    labels: {
                        "inicio": 0,
                        "brilhando": 1000,
                        "fim": 2000
                    },
                    data: [
                        [
                            "eid20",
                            "background-image",
                            1000,
                            500,
                            "linear",
                            "${Rectangle2}",
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]],
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]]
                        ],
                        [
                            "eid21",
                            "background-image",
                            1500,
                            500,
                            "linear",
                            "${Rectangle2}",
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]],
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]]
                        ]
                    ]
                }
            },
            "bt_5": {
                version: "6.0.0",
                minimumCompatibleVersion: "5.0.0",
                build: "6.0.0.400",
                scaleToFit: "none",
                centerStage: "none",
                resizeInstances: false,
                content: {
                    dom: [
                        {
                            rect: ['0px', '0px', '179px', '180px', 'auto', 'auto'],
                            stroke: [1, 'rgb(0, 0, 0)', 'solid'],
                            id: 'Rectangle3Copy3',
                            opacity: '1',
                            type: 'rect',
                            fill: ['rgba(200,200,200,1.00)', [43, [['rgba(164,164,164,1.00)', 0], ['rgba(255,255,255,1.00)', 100]]]]
                        },
                        {
                            rect: ['81px', '77px', '19px', '27px', 'auto', 'auto'],
                            textStyle: ['', '', '', '', 'none'],
                            font: ['Arial, Helvetica, sans-serif', [25, 'px'], 'rgba(0,0,0,1)', '400', 'none', 'normal', 'break-word', 'normal'],
                            id: 'Text2Copy4',
                            text: '<p style=\"margin: 0px;\">​5</p>',
                            align: 'center',
                            type: 'text'
                        }
                    ],
                    style: {
                        '${symbolSelector}': {
                            isStage: 'true',
                            rect: [undefined, undefined, '181px', '182px']
                        }
                    }
                },
                timeline: {
                    duration: 2043,
                    autoPlay: true,
                    labels: {
                        "inicio": 0,
                        "brilhando": 1000,
                        "fim": 2000
                    },
                    data: [
                        [
                            "eid18",
                            "background-image",
                            1000,
                            500,
                            "linear",
                            "${Rectangle3Copy3}",
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]],
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]]
                        ],
                        [
                            "eid19",
                            "background-image",
                            1500,
                            500,
                            "linear",
                            "${Rectangle3Copy3}",
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]],
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]]
                        ]
                    ]
                }
            },
            "bt_6": {
                version: "6.0.0",
                minimumCompatibleVersion: "5.0.0",
                build: "6.0.0.400",
                scaleToFit: "none",
                centerStage: "none",
                resizeInstances: false,
                content: {
                    dom: [
                        {
                            rect: ['0px', '0px', '291px', '283px', 'auto', 'auto'],
                            stroke: [1, 'rgb(0, 0, 0)', 'solid'],
                            id: 'Rectangle3Copy4',
                            opacity: '1',
                            type: 'rect',
                            fill: ['rgba(200,200,200,1)', [43, [['rgba(164,164,164,1.00)', 0], ['rgba(255,255,255,1.00)', 100]]]]
                        },
                        {
                            rect: ['138px', '129px', '19px', '27px', 'auto', 'auto'],
                            textStyle: ['', '', '', '', 'none'],
                            font: ['Arial, Helvetica, sans-serif', [25, 'px'], 'rgba(0,0,0,1)', '400', 'none', 'normal', 'break-word', 'normal'],
                            id: 'Text2Copy5',
                            text: '<p style=\"margin: 0px;\">​6</p>',
                            align: 'center',
                            type: 'text'
                        }
                    ],
                    style: {
                        '${symbolSelector}': {
                            isStage: 'true',
                            rect: [undefined, undefined, '293px', '285px']
                        }
                    }
                },
                timeline: {
                    duration: 2043,
                    autoPlay: false,
                    labels: {
                        "inicio": 0,
                        "brilhando": 1000,
                        "fim": 2000
                    },
                    data: [
                        [
                            "eid13",
                            "background-image",
                            1000,
                            500,
                            "linear",
                            "${Rectangle3Copy4}",
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]],
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]]
                        ],
                        [
                            "eid14",
                            "background-image",
                            1500,
                            500,
                            "linear",
                            "${Rectangle3Copy4}",
                            [43,[['rgba(219,95,95,1.00)',0],['rgba(220,143,143,1.00)',100]]],
                            [43,[['rgba(164,164,164,1.00)',0],['rgba(255,255,255,1.00)',100]]]
                        ]
                    ]
                }
            },
            "relogio": {
                version: "6.0.0",
                minimumCompatibleVersion: "5.0.0",
                build: "6.0.0.400",
                scaleToFit: "none",
                centerStage: "none",
                resizeInstances: false,
                content: {
                    dom: [
                        {
                            rect: ['0px', '0px', '66px', '16px', 'auto', 'auto'],
                            id: 'Rectangle2',
                            stroke: [1, 'rgb(0, 0, 0)', 'solid'],
                            type: 'rect',
                            fill: ['rgba(0,0,0,1.00)']
                        },
                        {
                            rect: ['6px', '1px', '58px', '17px', 'auto', 'auto'],
                            id: 'msn_saidaCopy',
                            text: '<p style=\"margin: 0px;\">​00:00:00</p>',
                            font: ['Arial, Helvetica, sans-serif', [14, 'px'], 'rgba(255,255,255,1.00)', 'normal', 'none', '', 'break-word', 'normal'],
                            type: 'text'
                        }
                    ],
                    style: {
                        '${symbolSelector}': {
                            isStage: 'true',
                            rect: [undefined, undefined, '68px', '18px']
                        }
                    }
                },
                timeline: {
                    duration: 1000,
                    autoPlay: true,
                    labels: {
                        "inicio": 0
                    },
                    data: [

                    ]
                }
            }
        };

    AdobeEdge.registerCompositionDefn(compId, symbols, fonts, scripts, resources, opts);

    if (!window.edge_authoring_mode) AdobeEdge.getComposition(compId).load("RetanguloOuro_edgeActions.js");
})("EDGE-603790615");
