# -*- coding: utf-8 -*-

import logging
from odoo import api, fields, models, _, exceptions
from odoo.addons.base.models.ir_module import request

import xw_cbase

_logger = logging.getLogger(__name__)


class IrModuleExtend(models.Model):
    """
    Ir Module Extend
    """
    _inherit = "ir.module.module"

    def dynamic_init_fields(attrs):
        """
        init fields
        """
        xw_cbase.setup_moudle_fields({
            'attrs': attrs,
            'models': models,
            'fields': fields,
            'api': api,
        })

    def button_immediate_install(self):
        """
        before install return a machine code wizard
        """
        _logger.info('User #%d triggered module installation !', self.env.uid)

        if request:
            request.allowed_company_ids = self.env.companies.ids

        terp = self.get_module_info(self.name)
        depends = terp.get('external_dependencies')
        if depends and self.name != 'xw_auth_server':
            for pydep in depends.get('python', []):
                if pydep == 'xw_cbase':
                    return {
                        'name': _('Machine Code'),
                        'type': 'ir.actions.act_window',
                        'res_model': 'xw_auth_base.reg_wizard',
                        'view_mode': 'form',
                        'target': 'new',
                        'context': {
                            'default_module_id': self.id,
                        }
                    }
        
        return super(IrModuleExtend, self).button_immediate_install()
