from ...can_database import *
from ...utils import *
from .c_config import *
from .c_writer import *

NUM_ALERTS = "NUM_{node}_{alert_type}S"
SET_ALERT_FUNC_NAME = "App_CanAlerts_Set{alert_type}"
GET_ALERT_FUNC_NAME = "App_CanAlerts_Get{alert_type}"
BOARD_HAS_ALERT_FUNC_NAME = "App_CanAlerts_BoardHas{alert_type}"
ANY_ALERT_FUNC_NAME = "App_CanAlerts_AnyBoardHas{alert_type}"
ALERT_BOARD_ENUM_NAME = "{node}_ALERT_BOARD"


class AppCanAlertsModule(CModule):
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node
        self._functions = self._public_functions()

    def _set_alert_func(self, alert_type: CanAlertType, comment: str):
        set_func = CFunc(
            SET_ALERT_FUNC_NAME.format(alert_type=alert_type),
            "void",
            args=[
                CVar(
                    "alert_id",
                    CTypesConfig.ALERT_ENUM.format(
                        node=self._node, alert_type=alert_type
                    ),
                ),
                CVar("set_alert", "bool"),
            ],
            comment=f"Set or clear a {comment}.",
        )

        set_func.body.start_switch("alert_id")
        for alert in self._db.node_alerts(self._node, alert_type):
            set_func.body.add_switch_case(alert)
            set_func.body.start_switch_case()
            set_func.body.add_line(
                f"App_CanTx_{self._node}_{alert_type}s_{alert}_Set(set_alert);"
            )
            set_func.body.add_switch_break()

        set_func.body.add_switch_default()
        set_func.body.start_switch_case()
        set_func.body.add_comment("Do nothing")
        set_func.body.add_switch_break()
        set_func.body.end_switch()
        return set_func

    def _get_alert_func(self, alert_type: CanAlertType, comment: str):
        get_func = CFunc(
            GET_ALERT_FUNC_NAME.format(alert_type=alert_type),
            "bool",
            args=[
                CVar(
                    "alert_id",
                    CTypesConfig.ALERT_ENUM.format(
                        node=self._node, alert_type=alert_type
                    ),
                )
            ],
            comment=f"Return whether or not a specific {comment} transmitted by this board is set.",
        )

        get_func.body.start_switch("alert_id")
        for alert in self._db.node_alerts(self._node, alert_type):
            get_func.body.add_switch_case(alert)
            get_func.body.start_switch_case()
            get_func.body.add_line(
                f"return App_CanTx_{self._node}_{alert_type}s_{alert}_Get();"
            )
            get_func.body.add_switch_break()

        get_func.body.add_switch_default()
        get_func.body.start_switch_case()
        get_func.body.add_line("return false;")
        get_func.body.add_switch_break()
        get_func.body.end_switch()
        return get_func

    def _board_has_alert_func(self, alert_type: CanAlertType, comment: str):
        has_alert = CFunc(
            BOARD_HAS_ALERT_FUNC_NAME.format(alert_type=alert_type),
            "bool",
            args=[
                CVar("board", CTypesConfig.CAN_ALERT_BOARD_ENUM),
            ],
            comment=f"Return whether or not a board has set a {comment}.",
        )
        has_alert.body.start_switch("board")

        nodes_with_alerts = [
            node for node in self._db.nodes if self._db.node_has_alert(node, alert_type)
        ]
        for node in nodes_with_alerts:
            has_alert.body.add_switch_case(
                ALERT_BOARD_ENUM_NAME.format(node=node.upper())
            )
            has_alert.body.start_switch_case()

            for alert in self._db.node_alerts_with_rx_check(
                node, self._node, alert_type
            ):
                if node == self._node:
                    has_alert.body.start_if(
                        f"App_CanTx_{node}_{alert_type}s_{alert}_Get()"
                    )
                else:
                    has_alert.body.start_if(
                        f"App_CanRx_{node}_{alert_type}s_{alert}_Get()"
                    )

                has_alert.body.add_line("return true;")
                has_alert.body.end_if()
                has_alert.body.add_line()

            has_alert.body.add_switch_break()

        has_alert.body.add_switch_default()
        has_alert.body.start_switch_case()
        has_alert.body.add_comment("Do nothing")
        has_alert.body.add_switch_break()
        has_alert.body.end_switch()

        has_alert.body.add_line()
        has_alert.body.add_line("return false;")
        return has_alert

    def _any_alert_set_func(self, alert_type: CanAlertType, comment: str):
        has_alert = CFunc(
            ANY_ALERT_FUNC_NAME.format(alert_type=alert_type),
            "bool",
            args=[],
            comment=f"Return whether or not any board has set a {comment}.",
        )

        nodes_with_alerts = [
            node for node in self._db.nodes if self._db.node_has_alert(node, alert_type)
        ]
        for node in nodes_with_alerts:
            has_alert.body.start_if(
                f"{BOARD_HAS_ALERT_FUNC_NAME.format(alert_type=alert_type)}({ALERT_BOARD_ENUM_NAME.format(node=node.upper())})"
            )
            has_alert.body.add_line("return true;")
            has_alert.body.end_if()
            has_alert.body.add_line()

        has_alert.body.add_line("return false;")
        return has_alert

    def _public_functions(self) -> List[CFunc]:
        funcs = []

        # Alert setters
        funcs.append(self._set_alert_func(CanAlertType.WARNING, "warning"))
        funcs.append(self._set_alert_func(CanAlertType.FAULT, "fault"))

        # Alert getters
        funcs.append(self._get_alert_func(CanAlertType.WARNING, "warning"))
        funcs.append(self._get_alert_func(CanAlertType.FAULT, "fault"))

        # Board-specific alert set checkers
        funcs.append(self._board_has_alert_func(CanAlertType.WARNING, "warning"))
        funcs.append(self._board_has_alert_func(CanAlertType.FAULT, "fault"))

        # All board alert set checkers
        funcs.append(self._any_alert_set_func(CanAlertType.WARNING, "warning"))
        funcs.append(self._any_alert_set_func(CanAlertType.FAULT, "fault"))

        return funcs

    def header(self):
        cw = CWriter()
        cw.add_preamble()
        cw.add_line()
        cw.add_line("#pragma once")

        # Add includes
        cw.add_line()
        cw.add_header_comment("Includes")
        cw.add_line()
        cw.add_include("<stdbool.h>")
        cw.add_include('"App_CanUtils.h"')

        # Add enums
        cw.add_line()
        cw.add_header_comment("Enums")
        cw.add_line()

        # Enum of CAN boards
        boards_enum = CEnum(CTypesConfig.CAN_ALERT_BOARD_ENUM)
        nodes_with_alerts = [
            node
            for node in self._db.nodes
            if any(
                [
                    self._db.node_has_alert(node, alert_type)
                    for alert_type in CanAlertType
                ]
            )
        ]
        for i, node in enumerate(nodes_with_alerts):
            boards_enum.add_value(
                CVar(ALERT_BOARD_ENUM_NAME.format(node=node.upper()), value=i)
            )
        cw.add_enum(boards_enum)

        # Enums of alerts set by this node
        for alert_type in CanAlertType:
            alerts_enum = CEnum(
                CTypesConfig.ALERT_ENUM.format(node=self._node, alert_type=alert_type)
            )
            for i, alert in enumerate(self._db.node_alerts(self._node, alert_type)):
                alerts_enum.add_value(CVar(alert, value=i))

            alerts_enum.add_value(
                CVar(
                    NUM_ALERTS.format(
                        node=self._node,
                        alert_type=pascal_to_screaming_snake_case(alert_type),
                    ),
                    value=len(alerts_enum.values),
                )
            )
            cw.add_enum(alerts_enum)
            cw.add_line()

        # Add function prototypes
        cw.add_line()
        cw.add_header_comment("Function Prototypes")
        cw.add_line()

        for func in self._functions:
            cw.add_function_prototype(func)
            cw.add_line()

        return str(cw)

    def source(self):
        cw = CWriter()
        cw.add_preamble()
        cw.add_line()

        # Add includes
        cw.add_line()
        cw.add_header_comment("Includes")
        cw.add_line()
        cw.add_include('"App_CanAlerts.h"')
        cw.add_include('"App_CanTx.h"')
        cw.add_include('"App_CanRx.h"')

        # Add function definitions
        cw.add_line()
        cw.add_header_comment("Function Definitions")
        cw.add_line()

        for func in self._functions:
            cw.add_function_definition(func)
            cw.add_line()

        return str(cw)