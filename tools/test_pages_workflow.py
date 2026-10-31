#!/usr/bin/env python3
"""Check that only verified main pushes can publish the generated site."""
from pathlib import Path
import unittest

import yaml


class PagesWorkflowTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        path = Path(__file__).resolve().parents[1] / '.github/workflows/verify.yml'
        cls.workflow = yaml.load(path.read_text(encoding='utf-8'), Loader=yaml.BaseLoader)
        cls.verify = cls.workflow['jobs']['verify']
        cls.deploy = cls.workflow['jobs']['deploy']

    def test_only_main_push_can_publish(self):
        self.assertEqual(self.workflow['on']['push']['branches'], ['main'])
        self.assertEqual(self.deploy['if'], "github.event_name == 'push' && github.ref == 'refs/heads/main'")
        self.assertEqual(self.deploy['needs'], 'verify')
        self.assertNotIn('always()', self.deploy['if'])

    def test_verification_failure_blocks_deploy(self):
        verifier = next(step for step in self.verify['steps'] if step.get('name') == 'Verify')
        self.assertEqual(verifier['with']['check'], 'true')
        for step in self.verify['steps']:
            self.assertNotEqual(step.get('continue-on-error'), 'true')

    def test_publish_uses_verified_site_and_scoped_permissions(self):
        upload = next(step for step in self.verify['steps'] if step.get('name') == 'Prepare GitHub Pages artifact')
        self.assertEqual(upload['with']['path'], '_site')
        self.assertEqual(upload['if'], self.deploy['if'])
        self.assertEqual(self.workflow['permissions'], {'contents': 'read'})
        self.assertEqual(self.deploy['permissions'], {'pages': 'write', 'id-token': 'write'})
        self.assertEqual(self.deploy['environment']['name'], 'github-pages')
        self.assertTrue(any(step.get('uses', '').startswith('actions/deploy-pages@') for step in self.deploy['steps']))


if __name__ == '__main__':
    unittest.main()
