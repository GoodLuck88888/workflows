
# -*- coding: utf-8 -*-

from odoo import models, fields, api, exceptions
import xw_cbase

from odoo.addons.base.models.ir_module import request
import logging

_logger = logging.getLogger(__name__)


class XwAuthBaseRegWizard(models.TransientModel):
    '''
    Auth Base Reg Wizard
    '''
    _name = 'xw_auth_base.reg_wizard'
    _description = 'reg wizard'

    def _default_machine_code(self):
        '''
        get default machine code
        '''
        return xw_cbase.get_machine_code()

    module_id = fields.Many2one('ir.module.module', string='Module', required=True, ondelete='cascade')
    machine_code = fields.Char(string='Machine Code', required=True, default=_default_machine_code)
    reg_code = fields.Char(string='Reg Code', required=True)

    def onRegist(self):
        '''
        on regist
        '''
        machine_code = self.machine_code
        reg_code = self.reg_code
        user_count = self.env['res.users'].search_count([])

        # do reg machine_code, reg_code
        try:
            result = xw_cbase.reg_module({
                'machineCode': machine_code,
                'regCode': reg_code,
                'self': self,
                'module': self.module_id,
                'fields': fields,
                'userCount': user_count,
                'xw_cbase': xw_cbase,
            })
            if not result:
                raise exceptions.ValidationError('Reg Failed !')
        except Exception as e:
            raise exceptions.ValidationError(e)

        _logger.info('User #%d triggered module installation', self.env.uid)
        if request:
            request.allowed_company_ids = self.env.companies.ids

        module_model = self.env['ir.module.module'].browse(self.module_id.id)
        return module_model._button_immediate_function(type(module_model).button_install)
